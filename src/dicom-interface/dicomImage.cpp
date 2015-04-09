#include <gdcmDataSet.h>
#include <gdcmFile.h>
#include <gdcmImageReader.h>
#include <gdcmImage.h>
#include <gdcmAnonymizer.h>
#include "dicom-interface/dicomImage.h"
#include <climits>

namespace Carna
{
namespace dicom
{

template< typename To, typename From >
To lexical_cast( const From& from )
{
    To to;
    std::stringstream ss;
    ss << from;
    ss >> to;
    return to;
}

template< typename T >
T sq( const T& x )
{
    return x * x;
}

//! Standard constructor
/*! 
 *	\author Moritz Hübner
 *	\date	June 2009
 */
DicomImage::DicomImage()
: _sdRefCounter(new short(1)), m_pRawImage(0), m_pUserImage(0), m_imageFlipped(false)
{}


//! Constructor with direct file loading
/*! 
 *	\param	filename	Path to the DICOM file which will be instantly loaded
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
DicomImage::DicomImage(std::string filename, bool bAnonymize)
: _sdRefCounter(new short(1)), m_pRawImage(0), m_pUserImage(0), m_imageFlipped(false)
{
    open(filename, bAnonymize);
}


//! Copy constructor
/*! Increments the shared data reference counter.
 *
 *	\param	source		Objects that should be copied
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
DicomImage::DicomImage(const DicomImage &source)
:	_sdRefCounter(source._sdRefCounter),
    m_iWidth(source.m_iWidth),
    m_iHeight(source.m_iHeight),
    m_bitsAllocated(source.m_bitsAllocated),
    m_bitsStored(source.m_bitsStored),
    m_dSpacingXY(source.m_dSpacingXY),
    m_dSpacingZ(source.m_dSpacingZ),
    m_recommendedWindowLevel(source.m_recommendedWindowLevel),
    m_recommendedWindowWidth(source.m_recommendedWindowWidth),
    m_currentWindowLevel(source.m_currentWindowLevel),
    m_currentWindowWidth(source.m_currentWindowWidth),
    m_imageFlipped(source.m_imageFlipped),
    m_distanceSource2Detector(source.m_distanceSource2Detector),
    m_pRawImage(source.m_pRawImage),
    m_pUserImage(source.m_pUserImage)
{
    ++*_sdRefCounter;
}


//! Standard destructor
/*! Image data is only released if this is the last instance of the DicomImage
 *
 *	\author Moritz Hübner
 *	\date	June 2009
 */
DicomImage::~DicomImage(void)
{
    if (--*_sdRefCounter == 0)
    {
        // Release image data
        if (m_pRawImage != 0)	{	delete[] m_pRawImage;	}
        if (m_pUserImage != 0)	{	delete[] m_pUserImage;	}
        delete _sdRefCounter;
    }
}


//! Assignment operator
/*! Releases the previous image reference and creates a new from the right-hand-side parameter.
 *
 *	\param	rhs		Right hand side of the operator
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
DicomImage& DicomImage::operator =(const DicomImage &rhs)
{
    // Check if this was the last reference on an existing image (actually, this is a simulated destructor)
    if (--*_sdRefCounter == 0)
    {
        // Release image data
        if (m_pRawImage != 0)	{	delete[] m_pRawImage;	}
        if (m_pUserImage != 0)	{	delete[] m_pUserImage;	}
        delete _sdRefCounter;
    }
    // Copy the class members
    _sdRefCounter = rhs._sdRefCounter;
    m_iWidth = rhs.m_iWidth;
    m_iHeight = rhs.m_iHeight;
    m_bitsAllocated = rhs.m_bitsAllocated;
    m_bitsStored = rhs.m_bitsStored;
    m_dSpacingXY = rhs.m_dSpacingXY;
    m_dSpacingZ = rhs.m_dSpacingZ;
    m_recommendedWindowLevel = rhs.m_recommendedWindowLevel;
    m_recommendedWindowWidth = rhs.m_recommendedWindowWidth;
    m_currentWindowLevel = rhs.m_currentWindowLevel;
    m_currentWindowWidth = rhs.m_currentWindowWidth;
    m_imageFlipped = rhs.m_imageFlipped;
    m_distanceSource2Detector = rhs.m_distanceSource2Detector;
    m_pRawImage = rhs.m_pRawImage;
    m_pUserImage = rhs.m_pUserImage;
    
    // Increment counter as this object is now a new reference to the image data
    ++*_sdRefCounter;

    // Return this object for operator chaining
    return (*this);
}


/**
The function OpenImage(std::string filename) opens Dicom formats
\author <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date June 2009
\param filename Filename of the image that should be loaded
\return TRUE if file could be opened
*/
bool DicomImage::open(std::string filename, bool bAnonymize)
{	
    // read dicom files with the gdcm library
    gdcm::ImageReader reader;   
    reader.SetFileName( filename.c_str() );
    if( !reader.Read() )
    {
        return false;
    }
    gdcm::File &file = reader.GetFile();

    if( bAnonymize )
    {
        gdcm::Anonymizer anonymizer;
        anonymizer.SetFile( file );
        anonymizer.RemovePrivateTags();
    }

    const gdcm::DataSet dataset = file.GetDataSet();
    const gdcm::Image &image = reader.GetImage();
    unsigned int ndim = image.GetNumberOfDimensions();
    const unsigned int *dims = image.GetDimensions();
    
    m_iWidth = dims[0];
    m_iHeight = dims[1];
    m_dSpacingXY = image.GetSpacing(0);
    m_dSpacingZ = image.GetSpacing(2);
    m_dPositionZ = readPositionZ( dataset );

    m_bitsAllocated = image.GetPixelFormat().GetBitsAllocated();
    m_bitsStored = image.GetPixelFormat().GetBitsStored();
    if (dataset(0x0018, 0x1110).GetByteValue())
        m_distanceSource2Detector = atof(dataset(0x0018, 0x1110).GetByteValue()->GetPointer());

    bool bRecommendedWindowExist = true;
    if (dataset(0x0028, 0x1050).GetByteValue())
        m_currentWindowLevel = m_recommendedWindowLevel = atoi(dataset(0x0028, 0x1050).GetByteValue()->GetPointer());
    else
        bRecommendedWindowExist = false;
    if (dataset(0x0028, 0x1051).GetByteValue())
        m_currentWindowWidth = m_recommendedWindowWidth = atoi(dataset(0x0028, 0x1051).GetByteValue()->GetPointer());
    else
        bRecommendedWindowExist = false;

    // Copy image data and prepare user image buffer
    if (m_pRawImage != 0)
        throw std::logic_error("Multiple calls of -open- are bad for the implicit data sharing. TODO: Get this to work! :-)");
    m_pRawImage = new char[image.GetBufferLength()]; 
    image.GetBuffer(m_pRawImage);
    if (m_pUserImage != 0)
        throw std::logic_error("Multiple calls of -open- are bad for the implicit data sharing. TODO: Get this to work! :-)");
    m_pUserImage = new unsigned char[m_iWidth*m_iHeight];


    // adjust the pixel values in HU values and create the viewable representation
    if (m_bitsAllocated == 8)
    {
        // If no recommended Windowing is stored, create a "full" window...
        if (!bRecommendedWindowExist)
        {
            int iMax = INT_MIN;
            int iMin = INT_MAX;
            // Look at all pixels stored to find min/max values
            for (int i = 0; i<m_iWidth*m_iHeight; i++) 
            {
                if (((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() < iMin)
                    iMin = ((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() + image.GetIntercept();
                if (((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() > iMax)
                    iMax = ((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() + image.GetIntercept();
            }
            m_currentWindowLevel = iMin + (iMax-iMin)/2;
            m_currentWindowWidth = iMax-iMin;
        }
        for (int i = 0; i<m_iWidth*m_iHeight; i++) 
        {
            m_pRawImage[i] = m_pRawImage[i] * image.GetSlope() + image.GetIntercept();
            m_pUserImage[i] = getWindowedPixel(m_pRawImage[i], m_currentWindowLevel, m_currentWindowWidth);
        }
    }
    else if (m_bitsAllocated == 16)
    {
        // If no recommended Windowing is stored, create a "full" window...
        if (!bRecommendedWindowExist)
        {
            int iMax = INT_MIN;
            int iMin = INT_MAX;
            // Look at all pixels stored to find min/max values
            for (int i = 0; i<m_iWidth*m_iHeight; i++) 
            {
                if (((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() < iMin)
                    iMin = ((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() + image.GetIntercept();
                if (((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() > iMax)
                    iMax = ((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept() + image.GetIntercept();
            }
            m_currentWindowLevel = iMin + (iMax-iMin)/2;
            m_currentWindowWidth = iMax-iMin;
        }
        for (int i = 0; i<m_iWidth*m_iHeight; i++) 
        {
            ((short*)(m_pRawImage))[i] = ((short*)(m_pRawImage))[i] * image.GetSlope() + image.GetIntercept(); 
            m_pUserImage[i] = getWindowedPixel(((short*)(m_pRawImage))[i], m_currentWindowLevel, m_currentWindowWidth);
        }
    }
    
    return true;
}

//! Constructs a DICOM image from ultrasound image data
/*! Uses the parameters given to construct a DicomImage with US as the modality
 *
 *	\param	width		Width of the image
 *	\param	height		Height of the image
 *	\param	imageData	Pointer to a data block which contains the image data which is copied for the DicomImage
 *
 *	\author	Stefan Bisplinghoff
 *	\data	November 2010
 */
void DicomImage::buildFromUSData(int width, int height, unsigned char* imageData)
{
    m_iWidth = width;
    m_iHeight = height;
    m_bitsAllocated = 8;
    m_bitsStored = 8;
    m_dSpacingZ = 0;		
    m_dSpacingXY = 1;		// May be something senseful after an ultrasound calibration
    m_recommendedWindowLevel = 128;
    m_recommendedWindowWidth = 128;
    m_currentWindowLevel = 128;
    m_currentWindowWidth = 128;
    m_imageFlipped = false;
    m_distanceSource2Detector = 0;

    m_pRawImage = new char[m_iWidth*m_iHeight];
    memcpy(m_pRawImage, imageData, m_iWidth*m_iHeight);
    m_pUserImage = new unsigned char[m_iWidth*m_iHeight];
    memcpy(m_pUserImage, imageData, m_iWidth*m_iHeight);
}

/**
The function GetWindowedPixel redefines the GrayValues of m_pRawImage to new ones which scaling range is 
smaller then the full spectrum as from -1024 to 3071
\author	        <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date		June 2009
\param		int iX			   x-position of requested pixel of the dicom image
\param		int iY			   y-position of requested pixel of the dicom image
\param		int iWindowStart   the lower border to adjust the gray value in a different range
\param		int iWindowEnd	   the upper border to adjust the gray value in a different range
\return		the new gray value for imaging different body parts specified to theire HU value
*/

unsigned char DicomImage::getWindowedPixel(short iValue, int iWindowLevel, int iWindowWidth) 
{
    if (iValue < iWindowLevel-iWindowWidth/2) ///< if pixelvalue is smaller than lower window border, than return black (=0)
    {
        return 0;
    }
    
    else if (iValue > iWindowLevel+iWindowWidth/2) ///< if pixelvalue is greater than upper window border, than return white (=255)
    {
        return 255;
    }
      
    else ///< else return a gray value between 0 and 255
    {		 
        return (iValue - (iWindowLevel-iWindowWidth/2)) * (255.0/(iWindowWidth-1));
    }	
}

/**
The function GetPixel returns the pixelvalue of the dicom image at the position x and y
\author	        <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date		June 2009
\param		x	x-position of requested pixel of the dicom image
\param		y	y-position of requested pixel of the dicom image
\return		Pixelvalue at the position x and y
*/
int DicomImage::getPixel(int iX, int iY) 
{
    return ((short*)(m_pRawImage))[iX+iY*m_iWidth];
}

double DicomImage::getSpacingZ() const
{
    return m_dSpacingZ;
}

double DicomImage::getPositionZ() const
{
    return m_dPositionZ;
}

void DicomImage::setPositionZ( const double& zPosition )
{
    m_dPositionZ = zPosition;
}

double DicomImage::getSpacingXY() const
{
    return m_dSpacingXY;
}

void DicomImage::setSpacingZ( double spacingZ )
{
    this->m_dSpacingZ = spacingZ;
}


//! Returns the viewable representation of the image.
/*! A const pointer to the 8 bit image date is returned. Therefore the data is meant to be read-only as multiple instances of the same image
 *	use the same data block for the raw and user image data.\n
 *	The user can modify the representation of the raw DICOM date by using the modification functions.
 *
 *	\return Pointer to the viewable image representation
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
const unsigned char *DicomImage::getImage() const
{
    return m_pUserImage;
}


//! Returns the viewable representation of the image.
/*! A pointer to the 8 bit image date is returned. In contrast to the other getImage, this function returns a normal pointer. Hence, the image
 *	data can be modified. We use implicit data sharing here, a deep copy of the dynamic data is performed if there are more than two references
 *	on it and the data pointer is changed. \n
 *	Keep in mind to evaluate the image data with the constant functions to avoid unnecessary data copying. \n
 *	The user can modify the representation of the raw DICOM date by using the modification functions. Be aware, that the use of any of the
 *	modification methods rewrites the user image data from the raw data including the given modification settings.
 *
 *	\return Pointer to the viewable image representation
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
unsigned char *DicomImage::getImage()
{
    _checkDeepCopy();
    return m_pUserImage;
}


//! Returns the raw data of the image
/*! A const pointer to the raw 16 bit image date is returned. Therefore the data is meant to be read-only as multiple instances of the same image
 *	use the same data block for the raw and user image data.\n
 *	This function is provided to access the full DICOM image data.
 *
 *	\return Pointer to the raw DICOM image data.
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
const char *DicomImage::getRawImage() const
{
    return m_pRawImage;
}


//! Returns the raw data of the image
/*! A pointer to the raw 16 bit image date is returned. In contrast to the other getRawImage, this function returns a normal pointer. Hence, the image
 *	data can be modified. We use implicit data sharing here, a deep copy of the dynamic data is performed if there are more than two references
 *	on it and the data pointer is changed. \n
 *	Keep in mind to evaluate the image data with the constant functions to avoid unnecessary data copying. \n
 *	This function is provided to access the full DICOM image data.
 *
 *	\return Pointer to the raw DICOM image data.
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
char *DicomImage::getRawImage()
{
    _checkDeepCopy();
    return m_pRawImage;
}


//! Performs a deep copy if there are multiple references to this image
/*! Used by getImage and getRawImage to maintain the implicit data sharing.
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::_checkDeepCopy()
{
    if (*_sdRefCounter > 1)
    {
        // Multiple reference, deep copy image data
        char *newRawImage = new char[m_iWidth*m_iHeight*m_bitsAllocated/8];
        memcpy(m_pRawImage, newRawImage, m_iWidth*m_iHeight*m_bitsAllocated/8);
        unsigned char *newUserImage = new unsigned char[m_iWidth*m_iHeight];
        memcpy(m_pUserImage, newUserImage, m_iWidth*m_iHeight);

        // Change the data pointer in this instance
        m_pRawImage = newRawImage;
        m_pUserImage = newUserImage;
        // Decrease ref counter and create a new for our own use as this is now an independent instance
        --*_sdRefCounter;
        _sdRefCounter = new short(1);
    }
}


//! Returns the window level recommended by the DICOM data
/*! 
 *	\return Recommended window level
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
short DicomImage::getRecommendedWindowLevel()
{
    return m_recommendedWindowLevel;
}


//! Returns the window width recommended by the DICOM data
/*! 
 *	\return Recommended window width
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
unsigned short DicomImage::getRecommendedWindowWidth()
{
    return m_recommendedWindowWidth;
}


//! Indicates if the image is flipped
/*! 
 *	\return TRUE if the used-defined image is flipped
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
bool DicomImage::isFlipped()
{
    return m_imageFlipped;
}


//! Returns the distance from the X-ray source to the detector
/*! 
 *	\return Distance in mm
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
double DicomImage::getDistanceSource2Detector()
{
    return m_distanceSource2Detector;
}


//! Resets the user-defined window settings to the recommended settings
/*! Current window settings are reverted to recommended ones and the user image is updated.
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::resetWindowSettings()
{
    m_currentWindowLevel = m_recommendedWindowLevel;
    m_currentWindowWidth = m_recommendedWindowWidth;
    applyUserWindow();
}


//! Calculates a updated windowed representation of the raw data
/*! The current window setting are used to calculate the representation. \n
 *	As any flipping is resetted with the new window settings, the flipUserImage method is called afterwards if necessary.
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::applyUserWindow()
{
    if (m_bitsAllocated == 8)
    {
        for (int i = 0; i<m_iWidth*m_iHeight; i++) 
        {
            m_pUserImage[i] = getWindowedPixel(m_pRawImage[i], m_currentWindowLevel, m_currentWindowWidth);
        }
    }
    else if (m_bitsAllocated == 16)
    {
        for (int i = 0; i<m_iWidth*m_iHeight; i++) 
        {
            m_pUserImage[i] = getWindowedPixel(((short*)(m_pRawImage))[i], m_currentWindowLevel, m_currentWindowWidth);
        }
    }

    // Flip image if necessary
    if (m_imageFlipped)
        flipUserImage();
}


//! Modifies the image buffer so that a flipped version is stored afterwards
/*! Performs a horizontally flipped version be mirroring each line.
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::flipUserImage()
{
    unsigned char swapPixel = 0;
    for (int y = 0; y < m_iHeight; ++y)
    {
        for (int x = 0; x < m_iWidth/2; ++x)
        {
            swapPixel = m_pUserImage[y*m_iWidth + x];
            m_pUserImage[y*m_iWidth + x] = m_pUserImage[(y+1)*m_iWidth - 1 - x];
            m_pUserImage[(y+1)*m_iWidth - 1 - x] = swapPixel;
        }
    }
}


//! Returns the image height
/*!
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
int DicomImage::getHeight() const
{
    return m_iHeight;
}


//! Returns the image width
/*!
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
int DicomImage::getWidth() const
{
    return m_iWidth;
}


//! Flips the image horizontally to receive the physically correct orientation
/*!
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::setFlipped(bool enable)
{
    if (m_imageFlipped != enable)
        flipUserImage();
    m_imageFlipped = enable;
}


//! Returns the current window level
/*!
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
short DicomImage::getCurrentWindowLevel() const
{
    return m_currentWindowLevel;
}


//! Returns the current window width
/*!
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
unsigned short DicomImage::getCurrentWindowWidth() const
{
    return m_currentWindowWidth;
}


//! Sets a new window level
/*!
 *	\param level	New window level for the viewable image representation
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::setWindowLevel(short level)
{
    if (m_currentWindowLevel != level)
    {
        m_currentWindowLevel = level;
        applyUserWindow();
    }
}


//! Sets a new window width
/*!
 *	\param level	New window width for the viewable image representation
 *
 *	\author Stefan Bisplinghoff
 *	\date	April 2010
 */
void DicomImage::setWindowWidth(unsigned short width)
{
    if (m_currentWindowWidth != width)
    {
        m_currentWindowWidth = width;
        applyUserWindow();
    }
}


double DicomImage::readPositionZ( const std::string& filename )
{
    gdcm::ImageReader reader;   
    reader.SetFileName( filename.c_str() );
    const gdcm::File &file = reader.GetFile(); 
    const gdcm::DataSet& dataSet = file.GetDataSet();
    if( !reader.Read() )
    {
        std::stringstream ss;
        ss << "Failed reading DICOM image file '" << filename << "'";
        throw std::runtime_error( ss.str() );
    }

    return readPositionZ( dataSet );
}


double DicomImage::readPositionZ( const gdcm::DataSet& dataSet )
{
    struct insufficient_header_information{};
    try
    {
        if( dataSet( 0x0020, 0x0032 ).IsEmpty() || dataSet( 0x0020, 0x0037 ).IsEmpty() )
        {
            throw insufficient_header_information();
        }
        
        const std::string image_position_str =
            std::string( dataSet( 0x0020, 0x0032 ).GetByteValue()->GetPointer() );

        std::string::size_type delimiter1 = image_position_str.find( '\\' );
        std::string::size_type delimiter2 = image_position_str.find( '\\', delimiter1 + 1 );

     // read image position

        const double position_x = lexical_cast< double >( image_position_str.substr( 0, delimiter1 ) );
        const double position_y = lexical_cast< double >( image_position_str.substr( delimiter1 + 1, delimiter2 - delimiter1 - 1 ) );
        const double position_z = lexical_cast< double >( image_position_str.substr( delimiter2 + 1 ) );

     // read image orientation

        const std::string image_orientation_str =
            std::string( dataSet( 0x0020, 0x0037 ).GetByteValue()->GetPointer() );

        double image_axis[ 6 ];
        std::string::size_type last_delimiter = -1;
        for( unsigned int i = 0; i < 6; ++i )
        {
            const std::string::size_type next_delimiter = image_orientation_str.find( '\\', last_delimiter + 1 );
            const std::string coordinate_str = next_delimiter != std::string::npos
                    ? image_orientation_str.substr( last_delimiter + 1, next_delimiter )
                    : image_orientation_str.substr( last_delimiter + 1 );

            image_axis[ i ] = lexical_cast< double >( coordinate_str );

            last_delimiter = next_delimiter;
        }

     // compute image normal

        double normal_x = image_axis[ 1 ] * image_axis[ 5 ] - image_axis[ 2 ] * image_axis[ 4 ];
        double normal_y = image_axis[ 2 ] * image_axis[ 3 ] - image_axis[ 0 ] * image_axis[ 5 ];
        double normal_z = image_axis[ 0 ] * image_axis[ 4 ] - image_axis[ 1 ] * image_axis[ 3 ];

     // normalize image normal

        const double normal_length = std::sqrt( sq( normal_x ) + sq( normal_y ) + sq( normal_z ) );
        if( normal_length < 1e-4f )
        {
            throw insufficient_header_information();
        }

        normal_x /= normal_length;
        normal_y /= normal_length;
        normal_z /= normal_length;

     // project image position vector on image normal vector

        const double position_and_normal_scalar_product
            = position_x * normal_x
            + position_y * normal_y
            + position_z * normal_z;

     // finish

        return position_and_normal_scalar_product;
    }
    catch( const insufficient_header_information& )
    {
        if ( !dataSet( 0x0020, 0x1041 ).IsEmpty() )
        {
            return atof( dataSet( 0x0020, 0x1041 ).GetByteValue()->GetPointer() );
        }
        else
        {
            return 0.;
        }
    }
}

}
}
