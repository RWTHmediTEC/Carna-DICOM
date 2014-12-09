/**
\brief Class for creating a vector used to number the different images, strung them together and gets access to them
The Class DicomSeries might be used to add a delete, and adding function for images to a programm. 
* \class DicomSeries
* \author <a href="mailto:huebner@hia.rwth-aachen.de@aol.com">Moritz Huebner</a>
* \date May - June 2009
* \ingroup DICOM_CLASSES
*/

#ifndef __DICOMSERIES_H__6487941264__
#define __DICOMSERIES_H__6487941264__

#include <vector>
#include <list>
#include <string>

namespace Carna
{
namespace dicom
{

class DicomImage;


class DicomSeriesOpeningController
{
private:
    DicomSeriesOpeningController( const DicomSeriesOpeningController& )
    {
    }
    DicomSeriesOpeningController& operator=( const DicomSeriesOpeningController& )
    {
        return *this;
    }
public:
    DicomSeriesOpeningController()
        : bAborted( false )
    {
    }
    virtual ~DicomSeriesOpeningController()
    {
    }
    bool isAborted() const
    {
        return bAborted;
    }

    virtual void processNewImage( const std::string& sPatientID
                                , const std::string& studyID
                                , const std::string& seriesID
                                , const std::string& fileName
                                , double zPosition ) = 0;

    virtual void dirOpened( unsigned int nFiles ) = 0;
    virtual void dirIterationProgress( unsigned int nFile ) = 0;
    virtual bool failedReadFile( const std::string& file, const std::vector< std::string >& succeeded ) = 0;
    virtual void hint( const std::string& ) = 0;
public:
    void abort()
    {
        bAborted = true;
    }
private:
    bool bAborted;
};


class DicomSeriesFileIterator
{
public:
    virtual ~DicomSeriesFileIterator()
    {
    }
    virtual std::string getNextFile() = 0;
    virtual bool hasNextFile() = 0;
};


class DicomSeries
{
public:
	DicomSeries(void);
	~DicomSeries(void);
	

	void addImage(DicomImage* pDicom); ///< function to strung together the numbers of images
	void deleteAll(); ///< delete the vector

	void deleteImage(int iID); ///< delete image at part int iID
	DicomImage* getImage(int iImageID); ///< Get image at part int iImageID
    const DicomImage* getImage(int iImageID) const;

    void openDicomDir( DicomSeriesFileIterator& it, DicomSeriesOpeningController& ctrl );

	//int getImageNumber();
	int width() const;
	int height() const;
	double spacingXY() const;
	double spacingZ() const;
	int size() const;
	char *getRawImage(int iID);							///< Returns the raw data of the image
	const char *getRawImage(int iID) const;
	std::vector <DicomImage *>* images();


	void setWidth(int iWidth);
	void setHeight(int iHeight);
	void setSpacingXY(double dspacingXY);
	void setSpacingZ(double dSpacingZ);

private:
	std::vector <DicomImage *> m_aImages; ///< vector m_aDicomSeries which contains objects of typ CDicomImage*
	int m_iWidth;
	int m_iHeight;
	double m_dSpacingZ; 
	double m_dSpacingXY; 	//int getImageNumber();
};

}
}

#endif // __DICOMSERIES_H__6487941264__
