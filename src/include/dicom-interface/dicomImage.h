/**
\brief Class for windowing gray values and returning the pixelvalue in the x and y scale
The Class DicomImage might be used to add a function for windowing
Pixel and one for creating a membervariable in a programm. 
It uses the gdcm library from Sourceforge.
* \class DicomImage
* \author <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
* \date May - June 2009
* \ingroup DICOM_CLASSES public DicomFile
*/

#ifndef __DICOMIMAGE_H__4856971254__
#define __DICOMIMAGE_H__4856971254__

namespace gdcm
{
    class DataSet;
}

namespace Carna
{
namespace dicom
{

// C++ includes
#include <string>
#include <stdexcept>


class DicomImage 
{
public:
    DicomImage();									///< Standard constructor
    DicomImage(std::string filename,
               bool bAnonymize=false);				///< Constructor with direct file loading
    DicomImage(const DicomImage &source);			///< Copy constructor
    ~DicomImage(void);								///< Standard destructor

    DicomImage& operator=(const DicomImage &rhs);	///< Assignment operator
private:
    short *_sdRefCounter;							///< Counts the references on this DicomImage for implicit data sharing of the image buffer
    void _checkDeepCopy();							///< Performs a deep copy if there are multiple references to this image

public:
    // File management
    bool open(std::string filename,
              bool bAnonymize=false);               ///< function to read dicom formats

    // Build from data
    void buildFromUSData(int width, int height, unsigned char* imageData);	///< Constructs a DICOM image from ultrasound image data

    // Access functions
    const unsigned char *getImage() const;			///< Returns the viewable representation of the image
    unsigned char *getImage();						///< Returns the viewable representation of the image
    const char *getRawImage() const;				///< Returns the raw data of the image
    char *getRawImage();							///< Returns the raw data of the image

    int getWidth() const;							///< Returns the image width
    int getHeight() const;							///< Returns the image height
    static unsigned char getWindowedPixel(short iValue, int iWindowLevel, int iWindowWidth); ///< adjust the gray values to a smaller range
    int getPixel(int iX, int iY);					///< creates the m_pRawImage variablei
    double getSpacingZ() const;						///< returns the z-coordinate of the origin
    double getSpacingXY() const;
    void setSpacingZ( double spacingZ );
    double getPositionZ() const;
    void setPositionZ( const double& );
    short getRecommendedWindowLevel();		///< Returns the window level recommended by the DICOM data
    unsigned short getRecommendedWindowWidth();		///< Returns the window width recommended by the DICOM data
    double getDistanceSource2Detector();			///< Returns the distance from the X-ray source to the detector

    // Modification functions
    void setFlipped(bool enable = true);			///< Flips the image horizontally to receive the physically correct orientation
    bool isFlipped();								///< Indicates if the image is flipped

    void setWindowLevel(short level);		///< Sets a new window level
    short getCurrentWindowLevel() const;    ///< Returns the current window level
    void setWindowWidth(unsigned short width);		///< Sets a new window width
    unsigned short getCurrentWindowWidth() const;	///< Returns the current window width
    void resetWindowSettings();						///< Resets the user-defined window settings to the recommended settings

    static double readPositionZ( const std::string& filename );
    static double readPositionZ( const gdcm::DataSet& dataset );

protected:
    // Image properties
    int m_iWidth;									///< x-size off the image written in the array by the dim[0] function of the gdcm library   
    int m_iHeight;									///< y-size off the image written in the array by the dim[1] function of the gdcm library   
    unsigned short m_bitsAllocated;					///< Indicates the number of bits allocated for the raw image data
    unsigned short m_bitsStored;					///< Indicates the number of bits stored for the raw image data
    double m_dSpacingZ;
    double m_dSpacingXY;
    double m_dPositionZ;
    short m_recommendedWindowLevel;		///< Window level recommended by the DICOM data
    unsigned short m_recommendedWindowWidth;		///< Window width recommended by the DICOM data
    short m_currentWindowLevel;			///< User-defined window level
    unsigned short m_currentWindowWidth;			///< User-defined window width
    bool m_imageFlipped;							///< Indicates if the user-defined image representation is flipped
    double m_distanceSource2Detector;				///< Distance from X-ray source to image detector

    // Image data
    char *m_pRawImage;								///< Array which contains every single pixel off all loaded images
    unsigned char *m_pUserImage;					///< User-defined viewable representation of the DICOM data, can be 

    // Internal functions
    void applyUserWindow();							///< Calculates a updated windowed representation of the raw data
    void flipUserImage();							///< Modifies the image buffer so that a flipped version is stored afterwards

};

}
}

#endif // __DICOMIMAGE_H__4856971254__
