#include "CarValidator.h"
#include "Rules/SedanContinentalRule.h"
#include "Rules/SuvToyotaRule.h"
#include "Rules/TruckWIARule.h"
#include "Rules/TruckMandoRule.h"
#include "Rules/BoschMismatchRule.h"

CarValidator::CarValidator() : m_ownsRules(true)
{
    // Order matches ValidationResult enum: FAIL_SEDAN_CONTINENTAL=1, FAIL_SUV_TOYOTA=2, ...
    m_rules.push_back(new SedanContinentalRule());
    m_rules.push_back(new SuvToyotaRule());
    m_rules.push_back(new TruckWIARule());
    m_rules.push_back(new TruckMandoRule());
    m_rules.push_back(new BoschMismatchRule());
}

CarValidator::CarValidator(std::vector<ICompatibilityRule*> rules)
    : m_rules(rules), m_ownsRules(false)
{
}

CarValidator::~CarValidator()
{
    if (m_ownsRules)
    {
        for (auto* r : m_rules)
            delete r;
    }
}

ValidationResult CarValidator::validate(const CarConfig& config) const
{
    for (int i = 0; i < (int)m_rules.size(); ++i)
    {
        if (m_rules[i]->isFailed(config))
        {
            // Map index to ValidationResult (only for default constructor ordering)
            // For injected rules, return a non-VALID result
            return static_cast<ValidationResult>(i + 1);
        }
    }
    return VALID;
}
