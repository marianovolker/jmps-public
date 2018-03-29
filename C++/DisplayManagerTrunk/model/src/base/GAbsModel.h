#if !defined(GAbsModel_h)
#define GAbsModel_h

class GAbsModel
{
public:
	GAbsModel();
	virtual ~GAbsModel();

    virtual void UpdateView(bool) = 0;

};

#endif
