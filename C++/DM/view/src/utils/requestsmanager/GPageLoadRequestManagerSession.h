#ifndef GPageLoadRequestManagerSession_H
#define	GPageLoadRequestManagerSession_H

#include <QString>

class GPageLoadRequestManagerSession
{

public:
    GPageLoadRequestManagerSession(unsigned int uiSessionId=0, QString sName="", bool bIsView=false);
    virtual ~GPageLoadRequestManagerSession();

    unsigned int GetId() const;
    const QString& GetName() const;
    bool IsView() const;

private:
    unsigned int m_iId;
    QString m_sName;
    bool m_bIsView;
};


inline unsigned int GPageLoadRequestManagerSession::GetId() const
{
	return m_iId;
}

inline const QString& GPageLoadRequestManagerSession::GetName() const
{
	return m_sName;
}

inline bool GPageLoadRequestManagerSession::IsView() const
{
	return m_bIsView;
}

#endif

