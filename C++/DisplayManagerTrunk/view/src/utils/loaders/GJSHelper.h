#ifndef GJSHelper_H
#define GJSHelper_H

#include <QStringList>
#include <QtWebKit/QWebFrame>

class GJSHelper
{
public:
	GJSHelper();
	~GJSHelper();

	inline QWebFrame * GetMainFrame();
	inline QStringList& GetJSCalls();
	inline QStringList& GetJSLibraries();

	inline void SetMainFrame( QWebFrame * );
	inline void SetJSCalls( const QStringList& );
	inline void SetJSLibraries( const QStringList& );

    void ExecuteJavaScript(const QString& oJScript);
    QString ReadHtmlFile(const QString& strPath);
    void LoadJSLibraries();
    void LoadJSCalls();


private:
    QWebFrame * m_pWebMainFrame;
    QStringList m_qslJsCalls;
    QStringList m_qslJsLibraries;
};


inline void GJSHelper::SetMainFrame( QWebFrame *pFrame )
{
	m_pWebMainFrame = pFrame;
}

inline void GJSHelper::SetJSCalls( const QStringList& qslJsCalls )
{
	m_qslJsCalls = qslJsCalls;
}

inline void GJSHelper::SetJSLibraries( const QStringList& qslJsLibraries )
{
	m_qslJsLibraries = qslJsLibraries;
}

inline QWebFrame * GJSHelper::GetMainFrame()
{
	return m_pWebMainFrame;
}

inline QStringList& GJSHelper::GetJSCalls()
{
	return m_qslJsCalls;
}

inline QStringList& GJSHelper::GetJSLibraries()
{
	return m_qslJsLibraries;
}

#endif
