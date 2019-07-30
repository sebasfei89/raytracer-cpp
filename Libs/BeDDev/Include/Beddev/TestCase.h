#pragma once

#include "beddev_export.h"

#include <functional>
#include <ostream>
#include <string>
#include <vector>

namespace beddev
{

class IExpression;

class TestCase
{
public:
    bool BEDDEV_EXPORT Run(std::ostream& os);

    using AssertionsSummary = std::tuple<uint32_t, uint32_t>;
    AssertionsSummary Assertions() const;

    struct Assertion
    {
        std::string test;
        std::string file;
        long line;
        bool result;
        std::string expanded;
    };

protected:
    BEDDEV_EXPORT TestCase(std::string const& desc, std::string const& fileAndLine, std::string const& category = "");

    virtual bool RunImpl() = 0;

    void AddFact(std::string const& fact) { m_facts.push_back(fact); }
    void AddAction(std::string const& action) { m_actions.push_back(action); }
    BEDDEV_EXPORT bool AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr);

private:
    std::string m_description;
    std::string m_category;
    std::string m_fileAndLine;
    std::vector<std::string> m_facts;
    std::vector<std::string> m_actions;
    std::vector<Assertion> m_tests;
};

} // EON beddev
