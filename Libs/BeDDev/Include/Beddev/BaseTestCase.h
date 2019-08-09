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
    virtual void Run(std::ostream& os, SessionSummary& summary, bool outputOnSuccess = false) = 0;
    virtual bool CanRun(std::string const& filter, std::string const& tags) const = 0;

    virtual std::string const& GetDescription() const = 0;
    virtual std::string const& GetTags() const = 0;

    virtual std::string const& GetFile() const = 0;
    virtual long GetLine() const = 0;
};

class BaseTestCase : public ITestCase
{
public:
    BEDDEV_EXPORT BaseTestCase(std::string const& desc, std::string const& file, long line, std::string const& category);

    std::string const& GetDescription() const override { return m_description; }
    std::string const& GetTags() const override { return m_tags; }
    std::string const& GetFile() const override { return m_file; }
    long GetLine() const override { return m_line; }

    BEDDEV_EXPORT void Run(std::ostream& os, SessionSummary& summary, bool outputOnSuccess = false) override;
    BEDDEV_EXPORT bool CanRun(std::string const& filter, std::string const& tags) const override;

protected:
    struct Assertion
    {
        BEDDEV_EXPORT Assertion(std::string const& expr, std::string const& file, long line, IExpression const& expression);

        std::string test;
        std::string file;
        long line;
        bool result;
        std::string expanded;
    };

    BEDDEV_EXPORT virtual void FormatHeader(std::ostream& os) const;
    BEDDEV_EXPORT void FormatFileAndLineInfo(std::ostream& os) const;
    BEDDEV_EXPORT void FormatItems(std::ostream& os, std::vector<std::string> const& items, std::string const& tag) const;
    BEDDEV_EXPORT void ReportConfigFailure(std::ostream& os, SessionSummary& summary, std::string const& errorMsg) const;
    BEDDEV_EXPORT void FormatAssertions(std::ostream& os, std::vector<Assertion> const& items, std::string const& tag) const;
    BEDDEV_EXPORT void FormatFailedTests(std::ostream& os, std::vector<Assertion> const& assertions, bool outputOnSuccess) const;
    BEDDEV_EXPORT virtual void FormatAssertionMessage(std::ostream& os, bool assertionResult) const;

    virtual void RunInternal(std::ostream& os, SessionSummary& summary, bool outputOnSuccess) = 0;
    virtual void AddFact(std::string const& fact) = 0;
    virtual void AddAction(std::string const& action) = 0;
    virtual bool AddTest(Assertion const& assertion) = 0;
    virtual void ReportFailure(std::ostream& os, bool outputOnSuccess) const = 0;

private:
    std::string m_description;
    std::string m_tags;
    std::vector<std::string> m_tagList;
    std::string m_file;
    long m_line;
};

}
