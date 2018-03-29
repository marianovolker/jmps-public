#ifndef GDisplayManagerConfig_H_
#define GDisplayManagerConfig_H_

#include <QMap>
#include <QString>
#include <QVariant>
#include <QMutex>

#include "utils/base/src/GException.h"
#include "utils/configreader/src/GConfigReader.h"
#include "utils/logger/src/GLogger.h"

class GDisplayManagerConfig
{
public:
	static QString const CSTRAlmostFailProofPersonalityName;

public:
	typedef enum
	{
		eDisplayManagerViewWidth 		  							=	0	,
		eDisplayManagerViewHeight		  							=	1	,
		eDisplayManagerTcpPort			  							=	2	,
		eDisplayManagerLogLevel			  							=	3	,
		eDisplayManagerLogFileName		  							=	4	,
		eDisplayManagerModelFileName	  							=	5	,
		eDisplayManagerJQueryLibraryFileName						=	6	,
		eDisplayManagerPersonalityDir								=	7	,
		eDisplayManagerPersonalityName								=	8	,
		eDisplayManagerEnableLoggingOfJavaScript					=	9	,
		eDisplayManagerImagesDir									=	10	,
		eDisplayManagerImageMappingsFileName						=	11	,
		eDisplayManagerApplyZoomFactor								=	12	,
		eDisplayManagerCacheSizeInPagesForTheView					=	13	,
		eDisplayManagerEnableLoggingOfLoadingPages					=	14	,
		eDisplayManagerPathForCertificateFile						=	15	,
		eDisplayManagerHTTPSSupport									=	16	,
		eDisplayManagerAutoAdjustmentViewSize						=	17	,
		eDisplayManagerShowEnvironment								=	18	,
		eDisplayManagerJSLibraryFileName							=	19	,
		eDisplayManagerMultipleConnections							=	20	,
		eDisplayManagerPathForVideoFiles							=	21	,
		eDisplayManagerVideoAppCommand								=	22	,
		eDisplayManagerVideoGstPipeline								=	23	,
		eDisplayManagerVideoVideoDecoderGstElement					=	24	,
		eDisplayManagerVideoAudioDecoderGstElement					=	25	,
		eDisplayManagerVideoRotation								=	26	,
		eDisplayManagerVideoMaxBufs									=	27	,
		eDisplayManagerVideoLateness								=	28	,
		eDisplayManagerVideoMuteCommand								=	29	,
		eDisplayManagerVideoStopEndSleep							=	30	,
		eDisplayManagerEnableAdvancedCache							=	31	,
		eDisplayManagerCacheLocation								=	32	,
		eDisplayManagerMaximumCacheSize								=	33  ,
		eDisplayManagerVideoDaemonCommand               			=   34  ,
		eDisplayManagerVideoDaemonClientCommand         			=   35  ,
		eDisplayManagerVideoDaemonClientCreatePipelineCommand   	=   36  ,
		eDisplayManagerVideoDaemonClientFilenameCommand 			=   37  ,
		eDisplayManagerVideoDaemonClientShowCommand 				=   38  ,
		eDisplayManagerVideoDaemonClientHideCommand 				=   39  ,
		eDisplayManagerVideoDaemonClientSetStateCommand 			=   40  ,
		eDisplayManagerVideoDaemonClientCommandTimeout  			=   41  ,
		eDisplayManagerMediaManagerType                 			=   42  ,
        eDisplayManagerVideoPlayCommandPathForVideoFile 			=   43	,
        eDisplayManagerMediaManagerForkFailedRestart				=	44

	} eDisplayManagerConfig;

private:
	void Insert( eDisplayManagerConfig eKey, const QString &strName, const QString &strDefaultValue );
	void Init()
	{
		Insert(eDisplayManagerViewWidth									, "DisplayManager.View.Width"									, "320");
		Insert(eDisplayManagerViewHeight								, "DisplayManager.View.Height"									, "240");
		Insert(eDisplayManagerTcpPort									, "DisplayManager.Port"											, "3601");
		Insert(eDisplayManagerLogLevel									, "DisplayManager.LogLevel"										, (QVariant::fromValue(static_cast<int>(GLogConfig::eLevel4))).toString());
		Insert(eDisplayManagerLogFileName								, "DisplayManager.LogFileName"									, "./DisplayManager.log");
		Insert(eDisplayManagerModelFileName								, "DisplayManager.InfoScreenModelFileName"						, "./Model.xml");
		Insert(eDisplayManagerJQueryLibraryFileName						, "DisplayManager.JQueryLibraryFileName"						, "./jquery-1.4.2.min.js");
		Insert(eDisplayManagerPersonalityDir							, "DisplayManager.PersonalityDir"								, "");
		Insert(eDisplayManagerPersonalityName							, "DisplayManager.PersonalityNameForInfoScreen"					, "");
		Insert(eDisplayManagerEnableLoggingOfJavaScript					, "DisplayManager.EnableLoggingOfJavaScript"					, "false");
		Insert(eDisplayManagerImagesDir									, "DisplayManager.ImagesDir"									, "");
		Insert(eDisplayManagerImageMappingsFileName						, "DisplayManager.ImageMappingsFileName"						, "./imagemappings.xml");
		Insert(eDisplayManagerApplyZoomFactor							, "DisplayManager.ApplyZoomFactor"								, "true");
		Insert(eDisplayManagerCacheSizeInPagesForTheView				, "DisplayManager.CacheSizeInPagesForTheView"					, "0");
		Insert(eDisplayManagerEnableLoggingOfLoadingPages				, "DisplayManager.EnableLoggingOfLoadingPages"					, "false");
		Insert(eDisplayManagerPathForCertificateFile					, "DisplayManager.PathForCertificateFile"						, "/usr/share/ca-certificates/gilbarco.com/gilbarcoca.crt");
		Insert(eDisplayManagerHTTPSSupport								, "DisplayManager.HTTPSSupport"									, "true");
		Insert(eDisplayManagerAutoAdjustmentViewSize					, "DisplayManager.AutoAdjustmentViewSize"						, "true");
		Insert(eDisplayManagerShowEnvironment							, "DisplayManager.ShowEnvironment"								, "false");
		Insert(eDisplayManagerJSLibraryFileName							, "DisplayManager.JSLibraryFileName"							, "./glibrary.js");
		Insert(eDisplayManagerMultipleConnections						, "DisplayManager.MultipleConnections"							, "false");
		Insert(eDisplayManagerPathForVideoFiles							, "DisplayManager.PathForVideoFiles"							, "/storage/applause/video/");
		Insert(eDisplayManagerVideoAppCommand							, "DisplayManager.VideoAppCommand"								, "/opt/gstreamer/bin/gst-launch");
		Insert(eDisplayManagerVideoGstPipeline							, "DisplayManager.VideoGstPipeline"								, "filesrc location=%1 name=fsrc ! qtdemux name=demux demux.video_00 ! queue ! %2 ! queue ! omapsink x=%3 y=%4 overlayWidth=%5 overlayHeight=%6 sync=true max-lateness=%7 rotation=%8 name=vsink demux.audio_00 ! queue max-size-buffers=%9 max-size-time=0 max-size-bytes=0 ! %10 ! alsasink");
		Insert(eDisplayManagerVideoVideoDecoderGstElement				, "DisplayManager.VideoVideoDecoderGstElement"					, "dmaidec_h264");
		Insert(eDisplayManagerVideoAudioDecoderGstElement				, "DisplayManager.VideoAudioDecoderGstElement"					, "mad");
		Insert(eDisplayManagerVideoRotation								, "DisplayManager.VideoRotation"								, "0");
		Insert(eDisplayManagerVideoMaxBufs								, "DisplayManager.VideoMaxBufs"									, "1000");
		Insert(eDisplayManagerVideoLateness								, "DisplayManager.VideoLateness"								, "20000000");
		Insert(eDisplayManagerVideoMuteCommand							, "DisplayManager.VideoMuteCommand"								, "/opt/crindbios/bin/gmixer");
		Insert(eDisplayManagerVideoStopEndSleep							, "DisplayManager.VideoStopEndSleep"							, "5");
		Insert(eDisplayManagerEnableAdvancedCache						, "DisplayManager.EnableAdvancedCache"							, "false");
		Insert(eDisplayManagerCacheLocation								, "DisplayManager.CacheLocation"								, "/tmp/");
		Insert(eDisplayManagerMaximumCacheSize							, "DisplayManager.MaximumCacheSize"								, "52428800");
		Insert(eDisplayManagerVideoDaemonCommand                        , "DisplayManager.VideoDaemonCommand"                         	, "/bin/gstd");
		Insert(eDisplayManagerVideoDaemonClientCommand                  , "DisplayManager.VideoDaemonClientCommand"                   	, "/bin/gst-client");
		Insert(eDisplayManagerVideoDaemonClientCreatePipelineCommand    , "DisplayManager.VideoDaemonClientCreatePipelineCommand"     	, "create");
		Insert(eDisplayManagerVideoDaemonClientFilenameCommand          , "DisplayManager.VideoDaemonClientFilenameCommand"           	, "-p /com/ridgerun/gstreamer/gstd/pipe0 set fsrc location string %1");
		Insert(eDisplayManagerVideoDaemonClientShowCommand              , "DisplayManager.VideoDaemonClientShowCommand"               	, "-p /com/ridgerun/gstreamer/gstd/pipe0 send-custom-event DOWNSTREAM_OOB showDisplay");
		Insert(eDisplayManagerVideoDaemonClientHideCommand              , "DisplayManager.VideoDaemonClientHideCommand"               	, "-p /com/ridgerun/gstreamer/gstd/pipe0 send-custom-event DOWNSTREAM_OOB hideDisplay");
		Insert(eDisplayManagerVideoDaemonClientSetStateCommand          , "DisplayManager.VideoDaemonClientSetStateCommand"           	, "-p /com/ridgerun/gstreamer/gstd/pipe0 %1");
		Insert(eDisplayManagerVideoDaemonClientCommandTimeout           , "DisplayManager.VideoDaemonClientCommandTimeout"            	, "2");
		Insert(eDisplayManagerMediaManagerType                          , "DisplayManager.MediaManagerType"                           	, "daemon"); // valid options - gstlaunch, daemon
        Insert(eDisplayManagerVideoPlayCommandPathForVideoFile          , "DisplayManager.PlayVideoCommandPathForVideoFile"             , "/var/opt/crindbios/video/");
        Insert(eDisplayManagerMediaManagerForkFailedRestart				, "DisplayManager.MediaManagerForkFailedRestart"				, "Enabled"); // valid options - Enabled, Disabled
	};

public:
	virtual ~GDisplayManagerConfig();
	static GDisplayManagerConfig * GetInstance() throw (GException);

	//Update/Retrieve from/to current configuration
	bool Set(eDisplayManagerConfig eKey, const QString& strNewValue);
	QString GetString( eDisplayManagerConfig eKey ) const;
	int GetInt( eDisplayManagerConfig eKey ) const;
	bool GetBool( eDisplayManagerConfig eKey ) const;
	void ShowCurrentConfiguration( ) const;

	//Update/Retrieve from/to default configuration from filesystem
	QString GetStringFromFileSystem( eDisplayManagerConfig eKey ) const;
	int GetIntFromFileSystem( eDisplayManagerConfig eKey ) const;
	bool GetBoolFromFileSystem( eDisplayManagerConfig eKey ) const;
	bool SetToFileSystem(eDisplayManagerConfig eKey, const QString& strNewValue);
	void ShowConfigurationFromFileSystem( ) const;

protected:
	GDisplayManagerConfig();

private:
	static GDisplayManagerConfig* m_pGDisplayManagerConfig;
	static const QString CSTRConfigEnvironmentPath;
	static const QString CSTRConfigFileName;
	static QMutex m_mutex;

private:
	typedef struct
	{
		QString strName;
		QString strDefaultValue;

	} stDisplayManagerConfig;

	QString GetName( eDisplayManagerConfig eKey ) const;
	QString GetDefaultValue( eDisplayManagerConfig eKey ) const;

	QMap<eDisplayManagerConfig, stDisplayManagerConfig> m_oConfigList;
	GConfigReader* m_pConfig;
	QString m_strFileName;
};


// Get/Set Values from Current configuration
//----------------------------------------------------------------------------------------------------------------------------------
#define SETCONF(x,y)\
		(\
			GDisplayManagerConfig::GetInstance()->Set(GDisplayManagerConfig::x, y)\
		)

#define GETCONF_NUM(x)\
		(\
			GDisplayManagerConfig::GetInstance()->GetInt(GDisplayManagerConfig::x)\
		)

#define GETCONF_STR(x)\
		(\
			GDisplayManagerConfig::GetInstance()->GetString(GDisplayManagerConfig::x)\
		)

#define GETCONF_BOOL(x)\
		(\
			GDisplayManagerConfig::GetInstance()->GetBool(GDisplayManagerConfig::x)\
		)

#define SHOW_CURRENT_CONFIGURATION()\
		(\
			GDisplayManagerConfig::GetInstance()->ShowCurrentConfiguration()\
		)
//----------------------------------------------------------------------------------------------------------------------------------


// Get/Set Values from filesystem
//----------------------------------------------------------------------------------------------------------------------------------
#define SETCONF_TO_FILESYSTEM(x,y)\
		(\
			GDisplayManagerConfig::GetInstance()->SetToFileSystem(GDisplayManagerConfig::x, y)\
		)

#define GETCONF_NUM_FROM_FILESYSTEM(x)\
		(\
			GDisplayManagerConfig::GetInstance()->GetIntFromFileSystem(GDisplayManagerConfig::x)\
		)

#define GETCONF_STR_FROM_FILESYSTEM(x)\
		(\
			GDisplayManagerConfig::GetInstance()->GetStringFromFileSystem(GDisplayManagerConfig::x)\
		)

#define GETCONF_BOOL_FROM_FILESYSTEM(x)\
		(\
			GDisplayManagerConfig::GetInstance()->GetBoolFromFileSystem(GDisplayManagerConfig::x)\
		)

#define SHOW_CONFIGURATION_FROM_FILESYSTEM()\
		(\
			GDisplayManagerConfig::GetInstance()->ShowConfigurationFromFileSystem()\
		)
//----------------------------------------------------------------------------------------------------------------------------------

#endif
