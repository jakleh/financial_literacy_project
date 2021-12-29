#include "FinancialSimulation.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <string.h>

//Function that takes money out of checking, then savings...
void accountDifference(FinancialIdentity *person, double difference) {   //EXTRA METHOD
  if (person->checking >= difference) {
    person->checking -= difference;
  }
  else if (person->savings >= difference){  //if not enough money in checking, then take from savings...
    int tempAmount = difference - person->checking;
    person->checking = 0;
    person->savings -= tempAmount;
  }
}

//Multiplies savings by the interest rate of some account...
void savingsPlacement(FinancialIdentity *person, double interestRate) {
  double rateScalar = interestRate + 1;
  person->savings *= rateScalar;
}

//Function that pays off debt each month over the course of a year...
void debt(FinancialIdentity *person, double interestRate, double addPay) {

  const int monthsInYear = 12;
  const double paymentScalar = 0.97; // (One minus the minimum payment rate of 0.03)

  double modifiedDebt; //Debt after monthly payment
  double debtDifference; //The monthly payment

  if (person->debt != 0.0) {
      person->yearsWithDebt += 1; //Incrementing years with debt...
  }

  for (int i = 0; i < monthsInYear; i++) {
    modifiedDebt = (person->debt * paymentScalar) - addPay;
    debtDifference = person->debt - modifiedDebt;

    if (modifiedDebt >= 0) {
      person->accruedDebt += debtDifference; //Keeping track of how much debt has been paid off for the write-up...
      person->debt = modifiedDebt;
      accountDifference(person, debtDifference);
    }

    else {
      debtDifference = person->debt;
      person->accruedDebt += debtDifference;
      person->debt = 0;
      accountDifference(person, debtDifference);
    }
  }

  double debtScalar = 1 + interestRate;
  person->debt *= debtScalar; //Multiplying debt by scalar...
}

//Function that withdraws yearly rent from account and increments "yearsRented" variable...
void rent(FinancialIdentity *person, double rentAmount) {
  const int yearlyRent= rentAmount * 12;
  //If they don't own a house...
  if (!person->ownsHouse) {
    accountDifference(person, yearlyRent);
    person->yearsRented += 1;
  }
}

//Function to pay off mortgage...
void house(FinancialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm) {
  int numPayments = mortgageTerm * 12;
  double monthlyInterest = interestRate / 12;

  //Discount factor is a function of the number of payments and the monthly interest...
  double discountFactor = (pow((1 + monthlyInterest), numPayments) - 1) / (monthlyInterest * pow((1 + monthlyInterest), numPayments));

  //Monthly payment is a function of the mortgage amount and the discount factor...
  double monthlyPayment = mortgageAmount / discountFactor;

  for (int i = 0; i < 12; i++) {
    if (person->mortgage >= monthlyPayment) {
      accountDifference(person, monthlyPayment);
      person->mortgage -= monthlyPayment;
      person->mortgage *= (1 + monthlyInterest);
    }

    else {
      accountDifference(person, person->mortgage);
      person->mortgage = 0;
    }
  }
}

//Function to gage whether someone is ready for a downpayment...
void houseActivity(FinancialIdentity *person, double downPayment) {
  if (person->savings >= downPayment) {
    person->savings -= downPayment;
    person->ownsHouse = true;
  }
}

//Uses houseActivity and house functions to deal with all transactions related to houses...
void finalHouseFunction(FinancialIdentity *person, double downPaymentRate, double mortgageInterestRate) {
  double simMortgageAmount;
  double simDownPayment;

  const int housePrice = 210000;

  //Number of years one has to pay off their mortgage...
  const int simMortgageTerm = 30;
  const int simRentAmount = 950;

  //The mortgage amount is the price of the house minus the downpayment, or housePrice - (downPaymentRate * housePrice) = housePrice * (1 - downPaymentRate)
  simMortgageAmount = (1 - downPaymentRate) * housePrice;
  simDownPayment = downPaymentRate * housePrice;

  //If the person doesn't own a house, determine whether they are ready to pay a downpayment...
  if (!person->ownsHouse) {
    houseActivity(person, simDownPayment);

    //If they are ready to pay a downpayment, then they've acquired a house and must pay of their mortgage...
    if (person->ownsHouse) {
      person->mortgage = simMortgageAmount;
      house(person, simMortgageAmount, mortgageInterestRate, simMortgageTerm);
    }

    //If they aren't, then they pay rent instead...
    else {
      rent(person, simRentAmount);
    }
  }

  //If the person already owns a house, then they pay off their mortgage...
  else {
    house(person, simMortgageAmount, mortgageInterestRate, simMortgageTerm);
  }
}

int *simulate(FinancialIdentity *person, double yearlySalary, bool financiallyLiterate) {
  int wealth;

  //Stores wealth for 41 individial years...
  int* wealthArray = (int*)malloc(41 * sizeof(int));

  person->accruedDebt = 0.0;

  for (int i = 0; i < 41; i++) {

    //Assets minus liabilities...
    wealth = (person->checking + person->savings) - (person->debt + person->mortgage);
    wealthArray[i] = wealth;

    person->checking += ((0.30) * yearlySalary);
    person->savings += ((0.20) * yearlySalary);

    if (financiallyLiterate) { //plugging in corresponding values for fl...
      savingsPlacement(person, 0.07);
      debt(person, 0.20, 100);

      finalHouseFunction(person, 0.20, 0.045);
    }

    else {  //plugging in corresponding values for nfl...
      savingsPlacement(person, 0.01);
      debt(person, 0.20, 1);

      finalHouseFunction(person, 0.050, 0.050);
    }
  }

  return wealthArray;
}

int main(void) { //Main method

  const double yearlySalary = 69000.0;

  FinancialIdentity fl = {5000.0, // savings 
                          0.0, // checking 
                          52500.0, // debt 
                          0, // mortgage 
                          0, // years with debt 
                          0, // years rented 
                          0.0, // debt paid 
                          false // owns house? 
                          };

  FinancialIdentity nfl = {5000.0, // savings 
                          0.0, // checking 
                          52500.0, // debt 
                          0, // mortgage 
                          0, // years with debt 
                          0, // years rented 
                          0.0, // debt paid 
                          false // owns house? 
                        };

  FILE *my_fl;
  FILE *my_nfl;

  int* firstArray = simulate(&fl, yearlySalary, 1);
  int* secondArray = simulate(&nfl, yearlySalary, 0);

  my_fl = fopen("C:\\output_fl.txt","w");

  fprintf(my_fl, "Financially Literate Person...");

  for (int i = 0; i < 41; i++) {
    fprintf(my_fl, "Year %d: $%d\n", i, firstArray[i]);
  }

  fclose(my_fl);

  my_nfl = fopen("C:\\output_nfl.txt","w");

  fprintf(my_nfl, "Financially Illiterate Person...");

  for (int i = 0; i < 41; i++) {
    fprintf(my_nfl, "Year %d: $%d\n", i, secondArray[i]);
  }

  fclose(my_nfl);

  return 0; //Final return statement
}
