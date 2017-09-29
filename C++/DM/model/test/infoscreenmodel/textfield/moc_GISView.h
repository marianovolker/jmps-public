#ifndef GISView_H_
#define GISView_H_

#include "utils/base/src/GException.h"

#include "model/src/infoscreenmodel/GTextField.h"
#include "model/src/infoscreenmodel/GSegment.h"

class GISView
{
public:
	static GISView* GetInstance() throw(GException);
	virtual ~GISView();
	void Init();

protected:
	GISView();

public:
	void Clear(bool bSynchronize = false);
	void Update(bool bSynchronize = false);
	void UpdateSection(GTextField * p, bool bSynchronize = false);
	void UpdateSection(GSegment * p, bool bSynchronize = false);

private:
  static GISView* m_pGISView;
};

#endif
