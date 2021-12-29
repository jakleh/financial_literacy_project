#include <stdbool.h>

typedef struct FinancialIdentity_struct {   //Struct with attributes...
  double savings;
  double checking;
  double debt;
  double accruedDebt;
  double mortgage;

  int yearsWithDebt;
  int yearsRented; 

  double debtPaid;

  bool ownsHouse;

} FinancialIdentity;

//Method stubs...
void savingsPlacement(FinancialIdentity *person, double interestRate);

void debt(FinancialIdentity *person, double interestRate, double addPay);

void rent(FinancialIdentity *person, double rentAmount);

void house(FinancialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm);

void houseActivity(FinancialIdentity *person, double downPayment);

void finalHouseFunction(FinancialIdentity *person, double downPaymentRate, double mortgageInterestRate);

int *simulate(FinancialIdentity *person, double yearlySalary, bool financiallyLiterate);

void accountDifference(FinancialIdentity *person, double difference);
