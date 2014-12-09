#include <gdcm/gdcmDataSet.h>
#include <gdcm/gdcmFile.h>
#include <gdcm/gdcmImageReader.h>
#include <gdcm/gdcmImage.h>
#include "dicom-interface/dicomSeries.h"
#include "dicom-interface/dicomImage.h"
#include <QDebug>

namespace Carna
{
namespace dicom
{

DicomSeries::DicomSeries(void)
{
}

DicomSeries::~DicomSeries(void)
{

}

/**
The function AddImage(CDicomImage* pDicom) saves the file IDs
\author	        <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date		June 2009
\param		CDicomImage* pDicom 
\return		m_aImages.push_back(pDicom) increase a vector about elements
*/

void DicomSeries::addImage(DicomImage* pDicom)
{
	m_aImages.push_back(pDicom); ///< m_aImages.push_back(pDicom) vektor where all iIDs are given back in pDicom 
}

/**
The function DeleteAll() deletes the vector. First the elements with a loop, then itself
\author	        <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date		June 2009	
*/

void DicomSeries::deleteAll()
{
	// for loop from i = 0 to size off m_aImages
	for (unsigned int i = 0; i<m_aImages.size(); i++) 
	{
	// delete element off place i
		delete m_aImages[i]; 
	}
	// delete vector
	m_aImages.clear(); 
}

/**
The function GetImage(int iImageID) choose the image with the number you wants and matches it to the right part
of m_pImageBuffer.
\author	        <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date		June 2009
\param		int iImageID number of an image
\return		a vector at element iImageID	
*/

DicomImage* DicomSeries::getImage(int iImageID)
{
	// vector of iImageID is matched to the image in m_pImageBuffer
	return m_aImages[iImageID];  
}

const DicomImage* DicomSeries::getImage(int iImageID) const
{
	// vector of iImageID is matched to the image in m_pImageBuffer
	return m_aImages[iImageID];  
}

/**
The function DeleteImage(int iID) deletes a single element off the vector.
\author	        <a href="mailto:huebner@hia.rwth-aachen.de">Moritz Huebner</a>
\date		June 2009
\param int iID number of an image
*/

void DicomSeries::deleteImage(int iID)
{
	// storage is released
	delete m_aImages[iID]; 
	// delete pointer from list
	m_aImages.erase(m_aImages.begin() + iID);
}


int DicomSeries::width() const
{
	return m_iWidth;
}
int DicomSeries::height() const
{
	return m_iHeight;
}
double DicomSeries::spacingXY() const
{
	return m_dSpacingXY;
}
double DicomSeries::spacingZ() const
{
	return m_dSpacingZ;
}
int DicomSeries::size() const
{
	return m_aImages.size();
}

void DicomSeries::setWidth(int iWidth)
{
	m_iWidth = iWidth;
}
void DicomSeries::setHeight(int iHeight)
{
	m_iHeight = iHeight;
}
void DicomSeries::setSpacingXY(double dSpacingXY)
{
	m_dSpacingXY = dSpacingXY;
}
void DicomSeries::setSpacingZ(double dSpacingZ)
{
	m_dSpacingZ = dSpacingZ;
}

char *DicomSeries::getRawImage(int iID)
{
	return m_aImages[iID]->getRawImage();
}

const char *DicomSeries::getRawImage(int iID) const
{
	return m_aImages[iID]->getRawImage();
}

std::vector <DicomImage *>* DicomSeries::images()
{
	return &m_aImages;
}

void DicomSeries::openDicomDir( DicomSeriesFileIterator& it, DicomSeriesOpeningController& ctrl )
{
	std::vector<std::string> sFilenameVector;

	while( it.hasNextFile() )
	{
        sFilenameVector.push_back( it.getNextFile() );
	}
	
	if( sFilenameVector.size() == 0 )
	{
		return;
	}

    ctrl.dirOpened( sFilenameVector.size() );

    std::vector< std::string > succeeded;
	for( int i = 0; i < sFilenameVector.size(); ++i )
	{
        if( ctrl.isAborted() )
        {
            break;
        }

        const std::string filename = sFilenameVector.at(i);
		// read dicom files with the gdcm library
		gdcm::ImageReader reader;   
		reader.SetFileName( filename.c_str() );
		if( !reader.Read() )
		{
            if( !ctrl.failedReadFile( filename, succeeded ) )
            {
			    return;
            }
		}
        else
        {
            succeeded.push_back( filename );

		    gdcm::File &file = reader.GetFile(); 
		    gdcm::DataSet& dataSet = file.GetDataSet();

		    std::string sPatientID, sStudyID, sSeriesID, sImageNumber;
		    if (dataSet(0x0010,0x0020).GetByteValue())
			    sPatientID = std::string(dataSet(0x0010,0x0020).GetByteValue()->GetPointer(), dataSet(0x0010,0x0020).GetByteValue()->GetLength()); // hole die Patienten ID des Bildes
		    else
			    sPatientID = "unknown";

		    if (dataSet(0x0020, 0x000D).GetByteValue())
			    sStudyID = std::string(dataSet(0x0020, 0x000D).GetByteValue()->GetPointer(), dataSet(0x0020, 0x000D).GetByteValue()->GetLength()); // hole die StudienID des Bildes
		    else
			    sStudyID = "unknown";

		    if (dataSet(0x0020, 0x000E).GetByteValue())
			    sSeriesID = std::string(dataSet(0x0020, 0x000E).GetByteValue()->GetPointer(), dataSet(0x0020, 0x000E).GetByteValue()->GetLength()); // hole die SerienID des Bildes
		    else
			    sSeriesID = "unknown";

		    if (dataSet(0x0020, 0x0013).GetByteValue())
			    sImageNumber = std::string(dataSet(0x0020, 0x0013).GetByteValue()->GetPointer(), dataSet(0x0020, 0x0013).GetByteValue()->GetLength());
		    else
                sImageNumber = "0";

            const double zPosition = DicomImage::readPositionZ( dataSet );

         // ----------------------------------------------------------------------------------

            ctrl.processNewImage( sPatientID, sStudyID, sSeriesID, filename, zPosition );
        }

        ctrl.dirIterationProgress( i + 1 );
	}
}

}
}
