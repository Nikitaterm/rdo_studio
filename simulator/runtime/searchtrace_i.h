#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class IDPTSearchTraceStatistics: public virtual rdo::counter_reference
{
public:
        enum Type
    {
        ST_TIMES,
        ST_COST,
        ST_MEMORY,
        ST_NODES,
        ST_NODES_EXPENDED,
        ST_NODES_FULL,
        ST_NODES_IN_GRAPH
    };
    virtual std::size_t getCalcCnt() const = 0;
    virtual std::size_t getCalcResFoundCnt() const = 0;
    virtual void getStatsDOUBLE(Type type, double& min, double& max, double& med) const = 0;
    virtual void getStatsRUINT(Type type, std::size_t&  min, std::size_t&  max, double& med) const = 0;
};
DECLARE_POINTER(IDPTSearchTraceStatistics)

#define DECLARE_IDPTSearchTraceStatistics                                                \
    virtual std::size_t getCalcCnt() const;                                              \
    virtual std::size_t getCalcResFoundCnt() const;                                      \
    virtual void getStatsDOUBLE(Type type, double& min, double& max, double& med) const; \
    virtual void getStatsRUINT(Type type, std::size_t&  min, std::size_t&  max, double& med) const;
