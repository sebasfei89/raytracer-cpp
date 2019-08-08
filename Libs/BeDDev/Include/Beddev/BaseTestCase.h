#pragma once

#include "beddev_export.h"

#include <string>
#include <vector>

namespace beddev
{

class IExpression;
class SessionSummary;

class ITestCase
{
public:
    virtual void Run(std::ostream& os, SessionSummary& summary) = 0;

    virtual std::string const& GetDescription() const = 0;
    virtual std::string const& GetCategory() const = 0;
    virtual std::string const& GetFile() const = 0;
    virtual long GetLine() const = 0;
};

class BaseTestCase : public ITestCase
{
public:
    BEDDEV_EXPORT BaseTestCase(std::string const& desc, std::string const& file, long line, std::string const& category);

    std::string const& GetDescription() const override { return m_description; }
    std::string const& GetCategory() const override { return m_category; }
    std::string const& GetFile() const override { return m_file; }
    long GetLine() const override { return m_line; }

    BEDDEV_EXPORT void Run(std::ostream& os, SessionSummary& summary) override;

protected:
    BEDDEV_EXPORT virtual void FormatHeader(std::ostream& os) const;
    BEDDEV_EXPORT void FormatFileAndLineInfo(std::ostream& os) const;
    BEDDEV_EXPORT void FormatItems(std::ostream& os, std::vector<std::string> const& items, std::string const& tag) const;

    virtual void RunInternal(std::ostream& os, SessionSummary& summary) = 0;
    virtual void AddFact(std::string const& fact) = 0;
    virtual void AddAction(std::string const& action) = 0;
    virtual bool AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr) = 0;
    virtual void ReportFailure(std::ostream& os) const = 0;

    struct Assertion
    {
        BEDDEV_EXPORT Assertion(std::string const& expr, std::string const& file, long line, bool result, std::string const& expanded, std::string const& errorMsg = "FAILED:");

        std::string test;
        std::string file;
        long line;
        bool result;
        std::string expanded;
        std::string errorMsg;
    };

    BEDDEV_EXPORT void FormatAssertions(std::ostream& os, std::vector<Assertion> const& items, std::string const& tag) const;
    BEDDEV_EXPORT void FormatFailedTests(std::ostream& os, std::vector<Assertion> const& assertions) const;

private:
    std::string m_description;
    std::string m_category;
    std::string m_file;
    long m_line;
};

}
