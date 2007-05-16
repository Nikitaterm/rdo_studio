#ifndef SIMTRACE_H
#define SIMTRACE_H

#include "rdotrace.h"

namespace rdoParse {
class RDODPTSome;
}

namespace rdoRuntime {

class RDOActivityRuntime;
class RDOBaseOperation;

class RDOSimulatorTrace: public RDOSimulator
{
friend class RDODecisionPointTrace;
friend class RDOOperationTrace;
friend class TreeRootTrace;
friend class TreeNodeTrace;
friend class RDOTrace;
friend class RDOResourceTrace;
friend class RDOIETrace;
friend class RDORuleTrace;
friend class RDOActivityTrace;
friend class RDOActivityRuntime;
friend class rdoParse::RDODPTSome;

private:
	int maxResourcesId;

	std::list<int> freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list< int > freeOperationsIds;

	int getFreeResourceId();
	void freeResourceId(int id);
	int getFreeOperationId(); 
	void freeOperationId(int id);

	int ieCounter;
	int activityCounter;
	int dptCounter;

	void addTemplateDecisionPoint(RDODecisionPointTrace *dp);
	void addTemplateOperation(RDOOperationTrace *op);
	void addTemplateIrregularEvent(RDOIETrace *ev);
	void addTemplateRule(RDORuleTrace *rule);

	unsigned int memory_current;
	unsigned int memory_max;

protected:
	RDOSimulatorTrace( RDORuntimeParent* _runtime ):
		RDOSimulator( _runtime ),
		dptCounter( 1 ),
		activityCounter( 1 ),
		ieCounter( 1 ),
		memory_current( 0 ),
		memory_max( 0 )
	{
	}
	int maxOperationId;
	void addTemplateBaseOperation( RDOBaseOperation* op );

	void incrementResourceIdReference(int id);

	virtual void preProcess();
	virtual void postProcess();
	void checkRSSDefinedResources();

	void onResourceErase( RDOResourceTrace* res );
	virtual std::list< RDOResourceTrace* > getTracebleResources() const = 0;

public:
	virtual RDOTrace* getTracer() = 0;
	virtual void rdoInit();

	void memory_insert( unsigned int mem ) {
		memory_current += mem;
		if ( memory_current > memory_max ) memory_max = memory_current;
	}
	void memory_remove( unsigned int mem ) {
		memory_current -= mem;
	}
	unsigned int memory_get() const {
		return memory_max;
	}
};

} // namespace rdoRuntime

#endif // SIMTRACE_H
