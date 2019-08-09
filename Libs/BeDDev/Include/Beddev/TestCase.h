#pragma once

#include "beddev_export.h"
#include "BaseTestCase.h"

namespace beddev
{

class TestCase : public BaseTestCase
{
public:
    BEDDEV_EXPORT TestCase(std::string const& desc, std::string const& file, long line, std::string const& category = "");

protected:
    BEDDEV_EXPORT void RunInternal(std::ostream& os, SessionSummary& summary, bool outputOnSuccess) override;
    virtual bool RunImpl() = 0;

    BEDDEV_EXPORT void AddFact(std::string const& fact) override;
    BEDDEV_EXPORT void AddAction(std::string const& action) override;
    BEDDEV_EXPORT bool AddTest(Assertion const& assertion) override;

    BEDDEV_EXPORT void ReportFailure(std::ostream& os, bool outputOnSuccess) const override;

    BEDDEV_EXPORT void SummarizeAssertions(SessionSummary& summary) const;

private:
    std::vector<std::string> m_facts;
    std::vector<std::string> m_actions;
    std::vector<Assertion> m_assertions;
};

} // EON beddev
