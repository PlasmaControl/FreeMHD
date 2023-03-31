#ifndef ADIOS2_QUERY_H
#define ADIOS2_QUERY_H

#include <ios>      //std::ios_base::failure
#include <iostream> //std::cout

#include <numeric>   // accumulate
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

//#include "adios2.h"
#include "adios2/common/ADIOSTypes.h"
#include "adios2/core/ADIOS.h"
#include "adios2/core/Engine.h"
#include "adios2/core/IO.h"
#include "adios2/core/Variable.h"
namespace adios2
{
namespace query
{
enum Op
{
    GT,
    LT,
    GE,
    LE,
    NE,
    EQ
};

enum Relation
{
    AND,
    OR,
    NOT
};

static adios2::query::Relation strToRelation(std::string relationStr) noexcept
{
    if ((relationStr.compare("or") == 0) || (relationStr.compare("OR") == 0))
        return adios2::query::Relation::OR;

    return adios2::query::Relation::AND; // default
}

static adios2::query::Op strToQueryOp(std::string opStr) noexcept
{
    if ((opStr.compare("lt") == 0) || (opStr.compare("LT") == 0))
        return adios2::query::Op::LT;
    if ((opStr.compare("gt") == 0) || (opStr.compare("GT") == 0))
        return adios2::query::Op::GT;
    if ((opStr.compare("ge") == 0) || (opStr.compare("GE") == 0))
        return adios2::query::Op::GE;
    if ((opStr.compare("le") == 0) || (opStr.compare("LE") == 0))
        return adios2::query::Op::LE;
    if ((opStr.compare("eq") == 0) || (opStr.compare("EQ") == 0))
        return adios2::query::Op::EQ;
    if ((opStr.compare("ne") == 0) || (opStr.compare("NE") == 0))
        return adios2::query::Op::NE;

    return adios2::query::Op::EQ; // default
}

static adios2::Dims split(const std::string &s, char delim)
{
    adios2::Dims dim;

    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        std::stringstream curr(item);
        size_t val;
        curr >> val;
        dim.push_back(val);
    }

    return dim;
}

//
// classes
//
class Range
{
public:
    adios2::query::Op m_Op;
    std::string m_StrValue;
    // void* m_Value = nullptr;

    // template<class T> bool Check(T val) const ;

    template <class T>
    bool CheckInterval(T &min, T &max) const;

    void Print() { std::cout << "===> " << m_StrValue << std::endl; }
}; // class Range

class RangeTree
{
public:
    void AddLeaf(adios2::query::Op op, std::string value)
    {
        Range range;
        range.m_Op = op;
        range.m_StrValue = value;

        m_Leaves.push_back(range);
    }

    void AddNode(RangeTree &node) { m_SubNodes.push_back(node); }

    void SetRelation(adios2::query::Relation r) { m_Relation = r; }

    void Print()
    {
        for (auto leaf : m_Leaves)
            leaf.Print();
        for (auto node : m_SubNodes)
            node.Print();
    }

    // template<class T>  bool Check(T value) const ;

    template <class T>
    bool CheckInterval(T &min, T &max) const;

    adios2::query::Relation m_Relation = adios2::query::Relation::AND;
    std::vector<Range> m_Leaves;
    std::vector<RangeTree> m_SubNodes;
}; // class RangeTree

class QueryBase
{
public:
    virtual ~QueryBase(){};
    virtual bool IsCompatible(const adios2::Box<adios2::Dims> &box) = 0;
    virtual void Print() = 0;
    virtual void BlockIndexEvaluate(adios2::core::IO &, adios2::core::Engine &,
                                    std::vector<Box<Dims>> &touchedBlocks) = 0;

    Box<Dims> GetIntersection(const Box<Dims> &box1,
                              const Box<Dims> &box2) noexcept
    {
        Box<Dims> b1 = adios2::helper::StartEndBox(box1.first, box1.second);
        Box<Dims> b2 = adios2::helper::StartEndBox(box2.first, box2.second);

        Box<Dims> result = adios2::helper::IntersectionBox(b1, b2);
        return adios2::helper::StartCountBox(result.first, result.second);
    }

    bool UseOutputRegion(const adios2::Box<adios2::Dims> &region)
    {
        if (!IsCompatible(region))
            return false;

        m_OutputRegion = region;
        BroadcastOutputRegion(region);
        return true;
    }

    virtual void
    BroadcastOutputRegion(const adios2::Box<adios2::Dims> &region) = 0;

    void ApplyOutputRegion(std::vector<Box<Dims>> &touchedBlocks,
                           const adios2::Box<Dims> &referenceRegion);

    adios2::Box<adios2::Dims> m_OutputRegion;

private:
    // bool ResetToOutputRegion(Box<Dims>& block);
};

class QueryVar : public QueryBase
{
public:
    QueryVar(const std::string &varName) : m_VarName(varName) {}
    ~QueryVar() {}

    std::string &GetVarName() { return m_VarName; }
    void BlockIndexEvaluate(adios2::core::IO &, adios2::core::Engine &,
                            std::vector<Box<Dims>> &touchedBlocks);
    void BroadcastOutputRegion(const adios2::Box<adios2::Dims> &region)
    {
        m_OutputRegion = region;
    }

    void Print() { m_RangeTree.Print(); }

    bool IsCompatible(const adios2::Box<adios2::Dims> &box)
    {
        if ((m_Selection.first.size() == 0) || (box.first.size() == 0))
            return true;

        if (box.first.size() != m_Selection.first.size())
            return false;

        for (int n = 0; n < box.second.size(); n++)
            if (box.second[n] != m_Selection.second[n])
                return false;

        return true;
    }

    void SetSelection(adios2::Dims &start, adios2::Dims &count)
    {
        m_Selection.first = start;
        m_Selection.second = count;
    }

    bool IsSelectionValid(adios2::Dims &varShape) const;

    bool TouchSelection(adios2::Dims &start, adios2::Dims &count) const;

    void LoadSelection(const std::string &startStr,
                       const std::string &countStr);

    void LimitToSelection(std::vector<Box<Dims>> &touchedBlocks)
    {
        for (auto it = touchedBlocks.begin(); it != touchedBlocks.end(); it++)
        {
            Box<Dims> overlap = GetIntersection(m_Selection, *it);
            // adios2::helper::IntersectionBox(m_Selection, *it);
            it->first = overlap.first;
            it->second = overlap.second;
        }
    }

    RangeTree m_RangeTree;
    adios2::Box<adios2::Dims> m_Selection;

    std::string m_VarName;

private:
}; // class QueryVar

class QueryComposite : public QueryBase
{
public:
    QueryComposite(adios2::query::Relation relation) : m_Relation(relation) {}
    ~QueryComposite()
    {
        for (auto n : m_Nodes)
            delete n;
        m_Nodes.clear();
    }

    void BroadcastOutputRegion(const adios2::Box<adios2::Dims> &region)
    {
        if (m_Nodes.size() == 0)
            return;

        for (auto n : m_Nodes)
            n->BroadcastOutputRegion(region);
    }

    void BlockIndexEvaluate(adios2::core::IO &, adios2::core::Engine &,
                            std::vector<Box<Dims>> &touchedBlocks);

    bool AddNode(QueryBase *v);

    void Print()
    {
        std::cout << " Composite query" << std::endl;
        for (auto n : m_Nodes)
            n->Print();
    }

    bool IsCompatible(const adios2::Box<adios2::Dims> &box)
    {
        if (m_Nodes.size() == 0)
            return false;
        return (m_Nodes[0])->IsCompatible(box);
    }

private:
    adios2::query::Relation m_Relation = adios2::query::Relation::AND;

    std::vector<QueryBase *> m_Nodes;
}; // class QueryComposite

/*
 */

} // namespace query
} //  namespace adiso2

#endif // define
