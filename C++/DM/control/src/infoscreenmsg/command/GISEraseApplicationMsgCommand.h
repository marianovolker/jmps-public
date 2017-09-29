//! GISEraseApplicationMsgCommand 
/*!
  Implementation of the Class GISEraseApplicationMsgCommand
*/

#if !defined(GISEraseApplicationMsgCommand_h)
#define GISEraseApplicationMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISEraseApplicationMsgCommand : public GISAbsCommand
{
public:

	GISEraseApplicationMsgCommand();
	virtual ~GISEraseApplicationMsgCommand();

	void DoExecute() throw (GException);

private:

    void DeleteFilesInDir(QString const &strImagesPath ) const;

    void DeleteImageFiles() const;

};
#endif
