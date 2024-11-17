#include "passphrase-checker.h"

bool isPassphraseStrong(const std::string& passphrase) {
    const std::string startAnchor = "^";

    const std::string lowercasePattern = "(?=.*[a-z])"; // At least one lowercase letter
    const std::string uppercasePattern = "(?=.*[A-Z])"; // At least one uppercase letter
    const std::string digitPattern = "(?=.*\\d)"; // At least one digit
    const std::string specialCharPattern = "(?=.*[^a-zA-Z\\d])"; // At least one special character
    const std::string minLengthPattern = ".{13,}"; // Minimum length of 13 characters

    const std::string endAnchor = "$";

    const Poco::RegularExpression strongPassphraseRegex(startAnchor + lowercasePattern + uppercasePattern + digitPattern +
        specialCharPattern + minLengthPattern + endAnchor);

    return strongPassphraseRegex.match(passphrase);
}