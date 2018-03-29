#ifndef GImageConverter_H_
#define GImageConverter_H_

#include <QThread>
#include <QMutex>
#include <QString>

#include"GImageJob.h"

#include "utils/logger/src/GLogger.h"
#include <QSharedPointer>


class GImageConverter : public QThread
{
	Q_OBJECT

public:

	static QSharedPointer<GImageConverter> GetInstance();

	void run();
	void Stop();

	void AddImageJob(GImageJob *oImageJob);

	static const QByteArray st_CPNGImageFormat;
	static const QByteArray st_CBMPImageFormat;

public:

	GImageConverter();
	~GImageConverter();


public:

	void Initialize(const float fScaleXFactor, const float fScaleYFactor);

	float GetScaleXFactor() const;
	float GetScaleYFactor() const;
	int GetMaxRetries() const;

	void SetScaleXFactor(const float fScaleXFactor);
	void SetScaleYFactor(const float fScaleYFactor);

	bool ConvertImagesIfNotConverted(const QString &strRawImagesDir, const QString &strConvertedImagesDir) const;

private:

	float m_fScaleXFactor;
	float m_fScaleYFactor;

	const int m_iMaxRetries; // max numbers of retries when converting one single image

	static QSharedPointer<GImageConverter> st_poGImageConverter;
	static QMutex st_oMutex;


	const QString ConvertedImagesExtension() const;
	const QByteArray ConvertedImagesFormat() const;

	const QString RawImagesFilter() const;

private slots:

	void ConvertImage(GImageJob *oImageJob);

signals:

	void AddJob(GImageJob *oImageJob);

};




inline const QString GImageConverter::RawImagesFilter() const
{
	return ("*." + st_CBMPImageFormat);
}


inline const QString GImageConverter::ConvertedImagesExtension() const
{
	return ("." + ConvertedImagesFormat());
}


inline const QByteArray GImageConverter::ConvertedImagesFormat() const
{
	return (st_CPNGImageFormat);
}


inline void GImageConverter::Initialize(const float fScaleXFactor, const float fScaleYFactor)
{
	SetScaleXFactor(fScaleXFactor);
	SetScaleYFactor(fScaleYFactor);
}


#endif
