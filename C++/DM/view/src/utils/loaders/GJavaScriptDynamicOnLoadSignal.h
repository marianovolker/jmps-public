#ifndef GJavaScriptDynamicOnLoadSignal_H_
#define GJavaScriptDynamicOnLoadSignal_H_

#include <QObject>
#include <QString>

class GJavaScriptDynamicOnLoadSignal
	:	public QObject
{
	Q_OBJECT

public:
	GJavaScriptDynamicOnLoadSignal(QString const & qksSignalName = "", unsigned int uiTimeoutMilliseconds = 25);
	~GJavaScriptDynamicOnLoadSignal();

	QString GetDynamicOnLoadSignalName();
	QString GetDynamicOnLoadCheckCode();
	QString GetDynamicOnLoadTriggerCode();

public slots:
	void Emit ();

signals:
	void Emited ();

private:
	QString m_sSignalName;
	unsigned int m_uiTimeoutMilliseconds;
};

#endif
