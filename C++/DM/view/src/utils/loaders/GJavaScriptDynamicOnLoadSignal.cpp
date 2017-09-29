#include "GJavaScriptDynamicOnLoadSignal.h"

#include "utils/logger/src/GLogger.h"

GJavaScriptDynamicOnLoadSignal::GJavaScriptDynamicOnLoadSignal(QString const & qksSignalName, unsigned int uiTimeoutMilliseconds)
	:	m_sSignalName(qksSignalName),
		m_uiTimeoutMilliseconds(uiTimeoutMilliseconds)
{
	LOG_LEVEL4("GJavaScriptDynamicOnLoadSignal()");

	static QString const qksDynamicOnLoadDefaultSignalName("JSPageLoadedSignal");
	if (m_sSignalName.isEmpty())
	{
		m_sSignalName = qksDynamicOnLoadDefaultSignalName;
	}
}

GJavaScriptDynamicOnLoadSignal::~GJavaScriptDynamicOnLoadSignal()
{
	LOG_LEVEL4("~GJavaScriptDynamicOnLoadSignal()");
}

void GJavaScriptDynamicOnLoadSignal::Emit()
{
	LOG_LEVEL4("Emit()");

	emit Emited();
}

QString GJavaScriptDynamicOnLoadSignal::GetDynamicOnLoadSignalName()
{
	LOG_LEVEL4("GetDynamicOnLoadSignalName()");

	return m_sSignalName;
}

QString GJavaScriptDynamicOnLoadSignal::GetDynamicOnLoadCheckCode()
{
	LOG_LEVEL4("GetDynamicOnLoadCheckCode()");

	static QString const qksDynamicOnLoadCheckCode
		(
			"(function()"
			"{"
				"window.DynamicOnLoad=new function DynamicOnLoad()"
				"{"
					"var timeout=0;"
					"var timerId=0;"
					"var callbackObject=new Array();"
					"this.OnLoadChecker=function()"
					"{"
						"timerId=0;"
						"for(var iI=0,bDone=true;(iI<document.images.length)&&(bDone==true);iI++)"
						"{"
							"if(document.images[iI].complete==false)"
							"{"
								"bDone=false;"
							"}"
						"}"
						"if(bDone)"
						"{"
							"while(callbackObject.length>0)"
							"{"
								"(callbackObject.shift())();"
							"}"
						"}"
						"else"
						"{"
							"timerId=setTimeout(arguments.callee,timeout);"
						"}"
					"};"
					"this.Stop=function()"
					"{"
						"if(timerId!=0)"
						"{"
							"clearTimeout(timerId);"
							"timerId=0;"
						"}"
						"return this;"
					"};"
					"this.Start=function(delay)"
					"{"
						"this.Stop();"
						"timeout=delay;"
						"timerId=setTimeout(this.OnLoadChecker,timeout);"
						"return this;"
					"};"
					"this.OnLoad=function(callback)"
					"{"
						"callbackObject.push(callback);"
						"return this;"
					"};"
				"};"
			"})();"
		);

	return qksDynamicOnLoadCheckCode;
}

QString GJavaScriptDynamicOnLoadSignal::GetDynamicOnLoadTriggerCode()
{
	LOG_LEVEL4("GetDynamicOnLoadTriggerCode()");

	static QString const qksDynamicOnLoadTriggerCode("if(DynamicOnLoad){DynamicOnLoad.OnLoad(function(){%1.Emit();}).Start(%2);}else{%1.Emit();}");
	return QString(qksDynamicOnLoadTriggerCode).arg(m_sSignalName).arg(m_uiTimeoutMilliseconds);
}
