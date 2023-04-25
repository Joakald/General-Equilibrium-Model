#include<vector>
#include<cassert>
#include<iostream>
#include<fstream>

struct Worker
{
public:
	// constructor
	Worker(double InitialRiskAversion,
		double InitialTimePreferece,
		double InitialLaborAversion,
		double InitialCashHolding)
		:
		RiskAversion(InitialRiskAversion),
		TimePreference(InitialTimePreferece),
		LaborAversion(InitialLaborAversion),
		CashHolding(InitialCashHolding)
	{}
public:
	// functions
	double GetInvestmentSupply(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// return supply of investments
		return (1.0 - exp(-InterestRate1 / RiskAversion))
			*(1.0 - TimePreference) * CashHolding;
	}
	double GetLaborSupply1(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// return supply of labor for firm 1
		return (1.0 - exp(-LaborAversion * (Wage1 / Price1)));
	}
	double GetLaborSupply2(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// return supply of labor for firm 2
		return (1.0 - exp(-LaborAversion * (Wage2 / Price1)));
	}
	double GetLaborSupply3(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// return supply of labor for firm 3
		return (1.0 - exp(-LaborAversion * (Wage3 / Price1)));
	}
	double GetLaborSupply4(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// return supply of labor for firm 4
		return (1.0 - exp(-LaborAversion * (Wage4 / Price1)));
	}
	double GetLaborSupply5(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// return supply of labor for firm 5
		return (1.0 - exp(-LaborAversion * (Wage5 / Price1)));
	}
	double GetConsumptionDemand(double Price1,
		double Wage1,
		double Wage2,
		double Wage3,
		double Wage4,
		double Wage5,
		double InterestRate1)
	{
		// set consumption demand given cash holdings
		return TimePreference * CashHolding / Price1;
	}
public:
	// variables
	double CashHolding = 0.0;
	double Consumption = 0.0;
	double Investment = 0.0;
public:
	// parameters
	double RiskAversion;
	double TimePreference;
	double LaborAversion;
};

struct Capitalist
{
public:
	// constructor
	Capitalist(double InitialRiskAversion,
		double InitialTimePreferece,
		double InitialCashHolding)
		:
		RiskAversion(InitialRiskAversion),
		TimePreference(InitialTimePreferece),
		CashHolding(InitialCashHolding)
	{}
public:
	// functions
	double GetInvestmentSupply(double Price1,
		double InterestRate1)
	{
		// get supply of investment
		return (1.0 - exp(-InterestRate1 / RiskAversion))
			* (1.0 - TimePreference) * CashHolding;
	}
	double GetConsumptionDemand(double Price1,
		double InterestRate1)
	{
		// return consumption demand given investment
		return TimePreference * CashHolding / Price1;
	}
public:
	// prefered variables
	double CashHolding = 0.0;
	double Consumption = 0.0;
	double Investment = 0.0;
public:
	// parameters
	double RiskAversion;
	double TimePreference;
};

struct Bank
{
public:
	// constructor
	Bank(double InitialRiskAversion)
		:
		RiskAversion(InitialRiskAversion)
	{}
public:
	// functions
	double GetCreditDemand(double InterestRate1,
		double InterestRate2)
	{
		if (InterestRate1 >= InterestRate2)
		{
			return 0.0;
		}
		// return credit demand
		return std::pow(InterestRate2 - InterestRate1, 1.0 - RiskAversion)
			/ (1.0 - RiskAversion);
	}
	double GetInvestmentSupply(double InterestRate1,
		double InterestRate2)
	{
		// return investment supply given credit
		return CashHolding;
	}
public:
	// variables
	double CashHolding = 0.0;
	double Investment = 0.0;
	double Credit = 0.0;
public:
	// parameters
	double RiskAversion;
};

struct Firm1
{
public:
	// constructor
	Firm1(double InitialTechnology,
		double InitialSubstitutability)
		:
		Technology(InitialTechnology),
		Substitutability(InitialSubstitutability)
	{}
public:
	// functions
	double GetCreditDemand(double Wage1,
		double Price1,
		double Price2,
		double InterestRate2)
	{
		// return demand for credit given profitability
		double Profitability = -(1.0 + InterestRate2) * (Price2 / Substitutability) + Price1 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price2 / Wage1), (1.0 - Substitutability));
		if (Profitability < 0.0)
		{
			return 0.0;
		}
		double PreferedCapital = Profitability;
		return (Price2 / Substitutability) * PreferedCapital;
	}
	double GetCapitalDemand(double Wage1,
		double Price1,
		double Price2)
	{
		// return demand for capital given cash holdings
		return Substitutability * CashHolding / Price2;
	}
	double GetLaborDemand(double Wage1, double Price2)
	{
		// return demand for labor given cash holdings and input stock
		return ((1.0 - Substitutability) / Substitutability) * (Price2 / Wage1) * InputStock;
	}
	void SetOutput()
	{
		// set output stock given input stock and hired labor
		OutputStock = Technology * std::pow(InputStock, Substitutability) * std::pow(HiredLabor, 1.0 - Substitutability);
	}
	double GetPreferedOutput(
		double Wage1, double Price1, double Price2, double InterestRate2)
	{
		// return prefered output given prices
		double Profitability = -(1.0 + InterestRate2) * (Price2 / Substitutability) + Price1 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price2 / Wage1), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		double PreferedLabor = PreferedCapital
			* (Substitutability / (1.0 - Substitutability)) * (Wage1 / Price2);
		return std::pow(PreferedCapital, Substitutability) 
			* std::pow(PreferedLabor, 1.0 - Substitutability);
	}
	double GetPreferedInput(
		double Wage1, double Price1, double Price2, double InterestRate2)
	{
		// return prefered input given prices
		double Profitability = -(1.0 + InterestRate2) * (Price2 / Substitutability) + Price1 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price2 / Wage1), (1.0 - Substitutability));
		return Profitability;
	}
	double GetPreferedLabor(
		double Wage1, double Price1, double Price2, double InterestRate2)
	{
		// return prefered labor given prices
		double Profitability = -(1.0 + InterestRate2) * (Price2 / Substitutability) + Price1 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price2 / Wage1), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		return PreferedCapital
			* ((1.0 - Substitutability) / Substitutability) * (Price2 / Wage1);
	}
public:
	// variables
	double OutputStock = 0.0;
	double InputStock = 0.0;
	double HiredLabor = 0.0;
	double Sails = 0.0;
	double CashHolding = 0.0;
	double WageFund = 0.0;
	double Credit = 0.0;
public:
	// parameters
	double Technology;
	double Substitutability;
};

struct Firm2
{
public:
	// constructor
	Firm2(double InitialTechnology,
		double InitialSubstitutability)
		:
		Technology(InitialTechnology),
		Substitutability(InitialSubstitutability)
	{}
public:
	// functions
	double GetCreditDemand(double Wage2,
		double Price2,
		double Price3,
		double InterestRate2)
	{
		// return initial plan, i.e. demand for credit
		double Profitability = -(1.0 + InterestRate2) * (Price3 / Substitutability) + Price2 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price3 / Wage2), (1.0 - Substitutability));
		if (Profitability < 0.0)
		{
			return 0.0;
		}
		double PreferedCapital = Profitability;
		return (Price3 / Substitutability) * PreferedCapital;
	}
	double GetCapitalDemand(double Wage2,
		double Price2,
		double Price3)
	{
		// return demand for capital given credit
		return Substitutability * CashHolding / Price3;
	}
	double GetLaborDemand(double Wage2, double Price3)
	{
		// return demand for labor given capital and credit
		return ((1.0 - Substitutability) / Substitutability) * (Price3 / Wage2) * InputStock;
	}
	void SetOutput()
	{
		// set output stock given input stock and hired labor
		OutputStock = Technology * std::pow(InputStock, Substitutability) * std::pow(HiredLabor, 1.0 - Substitutability);
	}
	double GetPreferedOutput(
		double Wage2, double Price2, double Price3, double InterestRate2)
	{
		// return prefered output given prices
		double Profitability = -(1.0 + InterestRate2) * (Price3 / Substitutability) + Price2 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price3 / Wage2), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		double PreferedLabor = PreferedCapital
			* (Substitutability / (1.0 - Substitutability)) * (Wage2 / Price3);
		return std::pow(PreferedCapital, Substitutability)
			* std::pow(PreferedLabor, 1.0 - Substitutability);
	}
	double GetPreferedInput(
		double Wage2, double Price2, double Price3, double InterestRate2)
	{
		// return prefered input given prices
		double Profitability = -(1.0 + InterestRate2) * (Price3 / Substitutability) + Price2 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price3 / Wage2), (1.0 - Substitutability));
		return Profitability;
	}
	double GetPreferedLabor(
		double Wage2, double Price2, double Price3, double InterestRate2)
	{
		// return prefered labor given prices
		double Profitability = -(1.0 + InterestRate2) * (Price3 / Substitutability) + Price2 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price3 / Wage2), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		return PreferedCapital
			* ((1.0 - Substitutability) / Substitutability) * (Price3 / Wage2);
	}
public:
	// variables
	double OutputStock = 0.0;
	double InputStock = 0.0;
	double HiredLabor = 0.0;
	double Sails = 0.0;
	double CashHolding = 0.0;
	double WageFund = 0.0;
	double Credit = 0.0;
public:
	// parameters
	double Technology;
	double Substitutability;
};

struct Firm3
{
public:
	// constructor
	Firm3(double InitialTechnology,
		double InitialSubstitutability)
		:
		Technology(InitialTechnology),
		Substitutability(InitialSubstitutability)
	{}
public:
	// functions
	double GetCreditDemand(double Wage3,
		double Price3,
		double Price4,
		double InterestRate2)
	{
		// return initial plan, i.e. demand for credit
		double Profitability = -(1.0 + InterestRate2) * (Price4 / Substitutability) + Price3 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price4 / Wage3), (1.0 - Substitutability));
		if (Profitability < 0.0)
		{
			return 0.0;
		}
		double PreferedCapital = Profitability;
		return (Price4 / Substitutability) * PreferedCapital;
	}
	double GetCapitalDemand(double Wage3,
		double Price3,
		double Price4)
	{
		// return demand for capital given credit
		return Substitutability * CashHolding / Price4;
	}
	double GetLaborDemand(double Wage3, double Price4)
	{
		// return demand for labor given capital and credit
		return ((1.0 - Substitutability) / Substitutability) * (Price4 / Wage3) * InputStock;
	}
	void SetOutput()
	{
		// set output stock given input stock and hired labor
		OutputStock = Technology * std::pow(InputStock, Substitutability) * std::pow(HiredLabor, 1.0 - Substitutability);
	}
	double GetPreferedOutput(
		double Wage3, double Price3, double Price4, double InterestRate2)
	{
		// return prefered output given prices
		double Profitability = -(1.0 + InterestRate2) * (Price4 / Substitutability) + Price3 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price4 / Wage3), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		double PreferedLabor = PreferedCapital
			* (Substitutability / (1.0 - Substitutability)) * (Wage3 / Price4);
		return std::pow(PreferedCapital, Substitutability)
			* std::pow(PreferedLabor, 1.0 - Substitutability);
	}
	double GetPreferedInput(
		double Wage3, double Price3, double Price4, double InterestRate2)
	{
		// return prefered input given prices
		double Profitability = -(1.0 + InterestRate2) * (Price4 / Substitutability) + Price3 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price4 / Wage3), (1.0 - Substitutability));
		return Profitability;
	}
	double GetPreferedLabor(
		double Wage3, double Price3, double Price4, double InterestRate2)
	{
		// return prefered labor given prices
		double Profitability = -(1.0 + InterestRate2) * (Price4 / Substitutability) + Price3 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price4 / Wage3), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		return PreferedCapital
			* ((1.0 - Substitutability) / Substitutability) * (Price4 / Wage3);
	}
public:
	// variables
	double OutputStock = 0.0;
	double InputStock = 0.0;
	double HiredLabor = 0.0;
	double Sails = 0.0;
	double CashHolding = 0.0;
	double WageFund = 0.0;
	double Credit = 0.0;
public:
	// parameters
	double Technology;
	double Substitutability;
};

struct Firm4
{
public:
	// constructor
	Firm4(double InitialTechnology,
		double InitialSubstitutability)
		:
		Technology(InitialTechnology),
		Substitutability(InitialSubstitutability)
	{}
public:
	// functions
	double GetCreditDemand(double Wage4,
		double Price4,
		double Price5,
		double InterestRate2)
	{
		// return initial plan, i.e. demand for credit
		double Profitability = -(1.0 + InterestRate2) * (Price5 / Substitutability) + Price4 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price5 / Wage4), (1.0 - Substitutability));
		if (Profitability < 0.0)
		{
			return 0.0;
		}
		double PreferedCapital = Profitability;
		return (Price5 / Substitutability) * PreferedCapital;
	}
	double GetCapitalDemand(double Wage4,
		double Price4,
		double Price5)
	{
		// return demand for capital given credit
		return Substitutability * CashHolding / Price5;
	}
	double GetLaborDemand(double Wage4, double Price5)
	{
		// return demand for labor given capital and credit
		return ((1.0 - Substitutability) / Substitutability) * (Price5 / Wage4) * InputStock;
	}
	void SetOutput()
	{
		// set output stock given input stock and hired labor
		OutputStock = Technology * std::pow(InputStock, Substitutability) * std::pow(HiredLabor, 1.0 - Substitutability);

	}
	double GetPreferedOutput(
		double Wage4, double Price4, double Price5, double InterestRate2)
	{
		// return prefered output given prices
		double Profitability = -(1.0 + InterestRate2) * (Price5 / Substitutability) + Price4 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price5 / Wage4), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		double PreferedLabor = PreferedCapital
			* (Substitutability / (1.0 - Substitutability)) * (Wage4 / Price5);
		return std::pow(PreferedCapital, Substitutability)
			* std::pow(PreferedLabor, 1.0 - Substitutability);
	}
	double GetPreferedInput(
		double Wage4, double Price4, double Price5, double InterestRate2)
	{
		// return prefered input given prices
		double Profitability = -(1.0 + InterestRate2) * (Price5 / Substitutability) + Price4 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price5 / Wage4), (1.0 - Substitutability));
		return Profitability;
	}
	double GetPreferedLabor(
		double Wage4, double Price4, double Price5, double InterestRate2)
	{
		// return prefered labor given prices
		double Profitability = -(1.0 + InterestRate2) * (Price5 / Substitutability) + Price4 *
			Technology * std::pow(((1.0 - Substitutability) / Substitutability)
				* (Price5 / Wage4), (1.0 - Substitutability));
		double PreferedCapital = Profitability;
		return PreferedCapital
			* ((1.0 - Substitutability) / Substitutability) * (Price5 / Wage4);
	}
public:
	// variables
	double OutputStock = 0.0;
	double InputStock = 0.0;
	double HiredLabor = 0.0;
	double Sails = 0.0;
	double CashHolding = 0.0;
	double WageFund = 0.0;
	double Credit = 0.0;
public:
	// parameters
	double Technology;
	double Substitutability;
};

struct Firm5
{
public:
	// constructor
	Firm5(double InitialTechnology)
		:
		Technology(InitialTechnology)
	{}
public:
	// functions
	double GetCreditDemand(double Wage5,
		double Price5,
		double InterestRate2)
	{
		// return initial plan, i.e. demand for credit
		double Profitability = Technology * Price5 - Wage5;
		if (Profitability < 0.0)
		{
			return 0.0;
		}
		double PreferedLabor = Profitability;
		return PreferedLabor * Wage5;
	}
	double GetLaborDemand(double Wage5)
	{
		// return demand for labor given credit
		return CashHolding / Wage5;
	}
	void SetOutput()
	{
		// set output stock given hired labor
		OutputStock = Technology * HiredLabor;
	}
	double GetPreferedOutput(
		double Wage5, double Price5, double InterestRate2)
	{
		// return prefered output given prices
		double Profitability = Technology * Price5 - Wage5;
		double PreferedLabor = Profitability;
		return PreferedLabor;

	}
	double GetPreferedLabor(
		double Wage5, double Price5, double InterestRate2)
	{
		// return prefered labor given prices
		double Profitability = Technology * Price5 - Wage5;
		return Profitability;
	}
public:
	// variables
	double OutputStock = 0.0;
	double HiredLabor = 0.0;
	double Sails = 0.0;
	double CashHolding = 0.0;
	double WageFund = 0.0;
	double Credit = 0.0;
public:
	// parameters
	double Technology;
};

double GetPercentageDiff(double Currentprice,
	double EquilibriumPrice, double Demand, double Supply,
	double ConvergenceEquilibrium, double ConvergenceExcess)
{
	// return based on demand and supply
	double DiffFactor = (Demand - Supply) / Demand;
	if (DiffFactor >= 0.0)
	{
		return  (1.0 - exp(ConvergenceExcess * DiffFactor));
	}
	else
	{
		return exp(ConvergenceExcess * DiffFactor) - 1.0;
	}
	// return based on equilibrium price
	//return ConvergenceEquilibrium * (EquilibriumPrice - Currentprice) / Currentprice;
}

int main()
{
	/// CREATE PARAMETERS, VARIABLES, VECTORS AND AGENTS ///
	// create system parameters
	int T = 100;
	double ConvergenceEquilibrium = 0.2;
	double ConvergenceExcess = 0.1;
	// create current price holders and set period 0 prices
	double Price1 = 6.0;
	double Price2 = 6.0;
	double Price3 = 6.0;
	double Price4 = 6.0;
	double Price5 = 6.0;
	double Wage1 = 1.0;
	double Wage2 = 1.0;
	double Wage3 = 1.0;
	double Wage4 = 1.0;
	double Wage5 = 1.0;
	double InterestRate1 = 0.1;
	double InterestRate2 = 0.2;
	// create next price holders
	double nPrice1 = 0.0;
	double nPrice2 = 0.0;
	double nPrice3 = 0.0;
	double nPrice4 = 0.0;
	double nPrice5 = 0.0;
	double nWage1 = 0.0;
	double nWage2 = 0.0;
	double nWage3 = 0.0;
	double nWage4 = 0.0;
	double nWage5 = 0.0;
	double nInterestRate1 = 0.0;
	double nInterestRate2 = 0.0;	
	// create price vectors
	std::vector<double> vPrice1(T, 0.0);
	std::vector<double> vPrice2(T, 0.0);
	std::vector<double> vPrice3(T, 0.0);
	std::vector<double> vPrice4(T, 0.0);
	std::vector<double> vPrice5(T, 0.0);
	std::vector<double> vWage1(T, 0.0);
	std::vector<double> vWage2(T, 0.0);
	std::vector<double> vWage3(T, 0.0);
	std::vector<double> vWage4(T, 0.0);
	std::vector<double> vWage5(T, 0.0);
	std::vector<double> vInterestRate1(T, 0.0);
	std::vector<double> vInterestRate2(T, 0.0);
	// create quantity vectors
	std::vector<double> vWorkerInvestment(T, 0.0);
	std::vector<double> vWorkerConsumption(T, 0.0);
	std::vector<double> vWorkerMadrassMoney(T, 0.0);
	std::vector<double> vCapitalistInvestment(T, 0.0);
	std::vector<double> vCapitalistConsumption(T, 0.0);
	std::vector<double> vCapitalistMadrassMoney(T, 0.0);
	std::vector<double> vBankCredit(T, 0.0);
	std::vector<double> vBankInvestment(T, 0.0);
	std::vector<double> vBankProfit(T, 0.0);
	std::vector<double> vFirm1Credit(T, 0.0);
	std::vector<double> vFirm1HiredLabor(T, 0.0);
	std::vector<double> vFirm1Output(T, 0.0);
	std::vector<double> vFirm1Sails(T, 0.0);
	std::vector<double> vFirm1Profit(T, 0.0);
	std::vector<double> vFirm2Credit(T, 0.0);
	std::vector<double> vFirm2HiredLabor(T, 0.0);
	std::vector<double> vFirm2Output(T, 0.0);
	std::vector<double> vFirm2Sails(T, 0.0);
	std::vector<double> vFirm2Profit(T, 0.0);
	std::vector<double> vFirm3Credit(T, 0.0);
	std::vector<double> vFirm3HiredLabor(T, 0.0);
	std::vector<double> vFirm3Output(T, 0.0);
	std::vector<double> vFirm3Sails(T, 0.0);
	std::vector<double> vFirm3Profit(T, 0.0);
	std::vector<double> vFirm4Credit(T, 0.0);
	std::vector<double> vFirm4HiredLabor(T, 0.0);
	std::vector<double> vFirm4Output(T, 0.0);
	std::vector<double> vFirm4Sails(T, 0.0);
	std::vector<double> vFirm4Profit(T, 0.0);
	std::vector<double> vFirm5Credit(T, 0.0);
	std::vector<double> vFirm5HiredLabor(T, 0.0);
	std::vector<double> vFirm5Output(T, 0.0);
	std::vector<double> vFirm5Sails(T, 0.0);
	std::vector<double> vFirm5Profit(T, 0.0);
	std::vector<double> vTotalHiredLabor(T, 0.0);
	std::vector<double> vTotalProfits(T, 0.0);
	std::vector<double> vTotalWagePayments(T, 0.0);
	// create agents and set initial agent parameter values
	Worker worker(0.5, 0.2, 1.0, 100.0);
	Capitalist capitalist(0.5, 0.2, 100.0);
	Bank bank(0.5);
	Firm1 firm1(5.0, 0.5);
	Firm2 firm2(5.0, 0.5);
	Firm3 firm3(5.0, 0.5);
	Firm4 firm4(5.0, 0.5);
	Firm5 firm5(5.0);
	/// END///

	// RUN MAIN LOOP
	for (int t = 0; t < T; t++)
	{
		/// CHANGE AGENT PARAMETERS ///

		/// END ///

		/// ADD CURRENT PRICES TO PRICE VECTORS
		vPrice1[t] = Price1;
		vPrice2[t] = Price2;
		vPrice3[t] = Price3;
		vPrice4[t] = Price4;
		vPrice5[t] = Price5;
		vWage1[t] = Wage1;
		vWage2[t] = Wage2;
		vWage3[t] = Wage3;
		vWage4[t] = Wage4;
		vWage5[t] = Wage5;
		vInterestRate1[t] = InterestRate1;
		vInterestRate2[t] = InterestRate2;
		/// END ///

		// create holders
		double Demand = 0.0;
		double Supply = 0.0;
		double MinDiff = 0.0;
		double PercentageDiff = 0.0;

		// SOLVE MARKET FOR WORKER AND CAPITALIST INVESTMENTS //		
		// find investments 
		double WorkerSupply = worker.GetInvestmentSupply(
			Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		double CapitalistSupply = capitalist.GetInvestmentSupply(Price1, InterestRate1);
		Supply = WorkerSupply + CapitalistSupply;
		Demand = bank.GetCreditDemand(InterestRate1, InterestRate2);
		if (Supply > Demand)
		{
			PercentageDiff = (Demand - Supply) / Supply;
			worker.Investment = (1.0 + PercentageDiff) * worker.GetInvestmentSupply(
					Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
			capitalist.Investment = (1.0 + PercentageDiff)
				* capitalist.GetInvestmentSupply(Price1, InterestRate1);
			bank.Credit = vWorkerInvestment[t] + vCapitalistInvestment[t];
		}
		else
		{
			worker.Investment = worker.GetInvestmentSupply(
					Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
			capitalist.Investment = capitalist.GetInvestmentSupply(
					Price1, InterestRate1);
			bank.Credit = worker.Investment + capitalist.Investment;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumInterestRate1 = 1.0;
		for (double r1 = 0.001; r1 < 1.0; r1 += 0.001)
		{
			Supply 
				= worker.GetInvestmentSupply(Price1, Wage1, Wage2, Wage3, Wage4, Wage5, r1)
				+ capitalist.GetInvestmentSupply(Price1, r1);
			Demand = bank.GetCreditDemand(r1, InterestRate2);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumInterestRate1 = r1;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(InterestRate1, EquilibriumInterestRate1,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nInterestRate1 = (1.0 + PercentageDiff) * InterestRate1;
		// transfer cash
		worker.CashHolding -= worker.Investment;
		capitalist.CashHolding -= capitalist.Investment;;
		bank.CashHolding += bank.Credit;
		// update quantity vectors
		vWorkerInvestment[t] = worker.Investment;
		vCapitalistInvestment[t] = capitalist.Investment;
		vBankCredit[t] = bank.Credit;
		// sanity checks
		assert(worker.CashHolding >= 0.0);
		assert(capitalist.CashHolding >= 0.0);
		assert(bank.Credit >= 0.0);
		// END //

		// SOLVE MARKET FOR BANK INVESTMENTS //
		// find investment
		Supply = bank.GetInvestmentSupply(InterestRate1, InterestRate2);
		double DemandFirm1 = firm1.GetCreditDemand(Wage1, Price1, Price2, InterestRate2);
		double DemandFirm2 = firm2.GetCreditDemand(Wage2, Price2, Price3, InterestRate2);
		double DemandFirm3 = firm3.GetCreditDemand(Wage3, Price3, Price4, InterestRate2);
		double DemandFirm4 = firm4.GetCreditDemand(Wage4, Price4, Price5, InterestRate2);
		double DemandFirm5 = firm5.GetCreditDemand(Wage5, Price5, InterestRate2);
		Demand = DemandFirm1 + DemandFirm2 + DemandFirm3 + DemandFirm4 + DemandFirm5;
		if (Supply > Demand)
		{
			firm1.Credit = DemandFirm1;
			firm2.Credit = DemandFirm2;
			firm3.Credit = DemandFirm3;
			firm4.Credit = DemandFirm4;
			firm5.Credit = DemandFirm5;
			bank.Investment = Demand;
		}
		else
		{
			double PercentageDiff =
				(Supply - Demand) / Demand;
			firm1.Credit = (1.0 + PercentageDiff) * DemandFirm1;
			firm2.Credit = (1.0 + PercentageDiff) * DemandFirm2;
			firm3.Credit = (1.0 + PercentageDiff) * DemandFirm3;
			firm4.Credit = (1.0 + PercentageDiff) * DemandFirm4;
			firm5.Credit = (1.0 + PercentageDiff) * DemandFirm5;
			bank.Investment = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumInterestRate2 = 1.0;
		for (double r2 = 0.001; r2 < 1.0; r2 += 0.001)
		{
			double Supply = bank.GetInvestmentSupply(InterestRate1, r2);
			double DemandFirm1 = firm1.GetCreditDemand(Wage1, Price1, Price2, r2);
			double DemandFirm2 = firm2.GetCreditDemand(Wage2, Price2, Price3, r2);
			double DemandFirm3 = firm3.GetCreditDemand(Wage3, Price3, Price4, r2);
			double DemandFirm4 = firm4.GetCreditDemand(Wage4, Price4, Price5, r2);
			double DemandFirm5 = firm5.GetCreditDemand(Wage5, Price5, r2);
			double Demand = DemandFirm1 + DemandFirm2 + DemandFirm3 + DemandFirm4 + DemandFirm5;
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumInterestRate2 = r2;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(InterestRate2, EquilibriumInterestRate2,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nInterestRate2 = (1.0 + PercentageDiff) * InterestRate2;
		// transfer cash
		bank.CashHolding -= bank.Investment;
		firm1.CashHolding += firm1.Credit;
		firm2.CashHolding += firm2.Credit;
		firm3.CashHolding += firm3.Credit;
		firm4.CashHolding += firm4.Credit;
		firm5.CashHolding += firm5.Credit;
		// update quantity vectors
		vBankInvestment[t] = bank.Investment;
		vFirm1Credit[t] = firm1.Credit;
		vFirm2Credit[t] = firm2.Credit;
		vFirm3Credit[t] = firm3.Credit;
		vFirm4Credit[t] = firm4.Credit;
		vFirm5Credit[t] = firm5.Credit;
		// sanity checks
		assert(bank.CashHolding >= 0.0);
		/// END ///

		// SOLVE MARKET FOR LABOR 5 //
		// find labor 5
		Supply = worker.GetLaborSupply5(Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		Demand = firm5.GetLaborDemand(Wage5);
		if (Supply > Demand)
		{
			firm5.HiredLabor = Demand;
		}
		else
		{
			firm5.HiredLabor = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumWage5 = 1.0;
		for (double w5 = 0.1; w5 < 100.0; w5 += 0.1)
		{
			Supply = worker.GetLaborSupply5(Price1, Wage1, Wage2, Wage3, Wage4, w5, InterestRate1);
			Demand = firm5.GetLaborDemand(w5);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumWage5 = w5;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Wage5, EquilibriumWage5,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nWage5 = (1.0 + PercentageDiff) * Wage5;
		// place money in wage fund
		firm5.CashHolding -= firm5.HiredLabor * Wage5;
		firm5.WageFund += firm5.HiredLabor * Wage5;
		// update quantity vector
		vFirm5HiredLabor[t] = firm5.HiredLabor;
		// sanity checks
		assert(firm5.HiredLabor >= 0.0);
		assert(firm5.WageFund >= 0.0);
		/// END ///

		/// COMMENCE PRODUCTION OF OUTPUT 5 ///
		// commence production
		firm5.SetOutput();

		/// SOLVE MARKET FOR OUTPUT 5 ///
		// find sails
		Supply = firm5.OutputStock;
		Demand = firm4.GetCapitalDemand(Wage4, Price4, Price5);
		if (Supply > Demand)
		{
			firm5.Sails = Demand;
		}
		else
		{
			firm5.Sails = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumPrice5 = 1.0;
		for (double p5 = 0.1; p5 < 100.0; p5 += 0.1)
		{
			Supply = firm5.GetPreferedOutput(Wage5, p5, InterestRate2);
			Demand = firm4.GetPreferedInput(Wage4, Price4, p5, InterestRate2);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumPrice5 = p5;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Price5, EquilibriumPrice5,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nPrice5 = (1.0 + PercentageDiff) * Price5;
		// update stock
		firm4.InputStock = firm5.Sails;
		// transfer cash
		firm4.CashHolding -= firm5.Sails * Price5;
		firm5.CashHolding += firm5.Sails * Price5;
		// update quantity vector
		vFirm5Sails[t] = firm5.Sails;
		vFirm5Output[t] = firm5.OutputStock;
		// sanity checks
		assert(firm5.Sails >= 0.0);
		/// END ///

		// SOLVE MARKET FOR LABOR 4 //
		// find labor 4
		Supply = worker.GetLaborSupply4(Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		Demand = firm4.GetLaborDemand(Wage4, Price5);
		if (Supply > Demand)
		{
			firm4.HiredLabor = Demand;
		}
		else
		{
			firm4.HiredLabor = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumWage4 = 1.0;
		for (double w4 = 0.1; w4 < 100.0; w4 += 0.1)
		{
			Supply = worker.GetLaborSupply4(Price1, Wage1, Wage2, Wage3, Wage4, w4, InterestRate1);
			Demand = firm5.GetLaborDemand(w4);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumWage4 = w4;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Wage4, EquilibriumWage4,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nWage4 = (1.0 + PercentageDiff) * Wage4;
		// place money in wage fund
		firm4.CashHolding -= firm4.HiredLabor * Wage4;
		firm4.WageFund += firm4.HiredLabor * Wage4;
		// update quantity vector
		vFirm4HiredLabor[t] = firm4.HiredLabor;
		// sanity checks
		assert(firm5.HiredLabor >= 0.0);
		assert(firm5.WageFund >= 0.0);
		/// END ///

		/// COMMENCE PRODUCTION OF OUTPUT 5 ///
		// commence production
		firm4.SetOutput();

		/// SOLVE MARKET FOR OUTPUT 4 ///
		// find sails
		Supply = firm4.OutputStock;
		Demand = firm3.GetCapitalDemand(Wage3, Price3, Price4);
		if (Supply > Demand)
		{
			firm4.Sails = Demand;
		}
		else
		{
			firm4.Sails = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumPrice4 = 1.0;
		for (double p4 = 0.1; p4 < 100.0; p4 += 0.1)
		{
			Supply = firm4.GetPreferedOutput(Wage4, p4, Price5, InterestRate2);
			Demand = firm3.GetPreferedInput(Wage3, Price3, p4, InterestRate2);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumPrice4 = p4;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Price4, EquilibriumPrice4,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nPrice4 = (1.0 + PercentageDiff) * Price4;
		// update stock
		firm3.InputStock = firm4.Sails;
		// transfer cash
		firm3.CashHolding -= firm4.Sails * Price4;
		firm4.CashHolding += firm4.Sails * Price4;
		// update quantity vector
		vFirm4Sails[t] = firm4.Sails;
		vFirm4Output[t] = firm4.OutputStock;
		// sanity checks
		assert(firm4.CashHolding >= 0.0);
		assert(firm3.CashHolding >= 0.0);
		assert(firm4.Sails >= 0.0);
		/// END ///

		// SOLVE MARKET FOR LABOR 3 //
		// find labor 3
		Supply = worker.GetLaborSupply3(Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		Demand = firm3.GetLaborDemand(Wage3, Price4);
		if (Supply > Demand)
		{
			firm3.HiredLabor = Demand;
		}
		else
		{
			firm3.HiredLabor = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumWage3 = 1.0;
		for (double w3 = 0.1; w3 < 100.0; w3 += 0.1)
		{
			Supply = worker.GetLaborSupply3(Price1, Wage1, Wage2, Wage3, Wage4, w3, InterestRate1);
			Demand = firm3.GetLaborDemand(w3, Price4);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumWage3 = w3;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Wage3, EquilibriumWage3,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nWage3 = (1.0 + ConvergenceEquilibrium * PercentageDiff) * Wage3;
		// place money in wage fund
		firm3.CashHolding -= firm3.HiredLabor * Wage3;
		firm3.WageFund += firm3.HiredLabor * Wage3;
		// update quantity vector
		vFirm3HiredLabor[t] = firm3.HiredLabor;
		// sanity checks
		assert(firm3.HiredLabor >= 0.0);
		assert(firm3.WageFund >= 0.0);
		/// END ///

		/// COMMENCE PRODUCTION OF OUTPUT 3 ///
		// commence production
		firm3.SetOutput();

		/// SOLVE MARKET FOR OUTPUT 3 ///
		// find sails
		Supply = firm3.OutputStock;
		Demand = firm2.GetCapitalDemand(Wage2, Price2, Price3);
		if (Supply > Demand)
		{
			firm3.Sails = Demand;
		}
		else
		{
			firm3.Sails = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumPrice3 = 1.0;
		for (double p3 = 0.1; p3 < 100.0; p3 += 0.1)
		{
			Supply = firm3.GetPreferedOutput(Wage3, p3, Price4, InterestRate2);
			Demand = firm2.GetPreferedInput(Wage2, Price2, p3, InterestRate2);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumPrice3 = p3;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Price3, EquilibriumPrice3,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nPrice3	= (1.0 + ConvergenceEquilibrium * PercentageDiff) * Price3;
		// update stock
		firm2.InputStock = firm3.Sails;
		// transfer cash
		firm2.CashHolding -= firm3.Sails * Price3;
		firm3.CashHolding += firm3.Sails * Price3;
		// update quantity vector
		vFirm3Sails[t] = firm3.Sails;
		vFirm3Output[t] = firm3.OutputStock;
		// sanity checks
		assert(firm3.CashHolding >= 0.0);
		assert(firm2.CashHolding >= 0.0);
		assert(firm3.Sails >= 0.0);
		/// END ///

		// SOLVE MARKET FOR LABOR 2 //
		// find labor 2
		Supply = worker.GetLaborSupply2(Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		Demand = firm2.GetLaborDemand(Wage2, Price3);
		if (Supply > Demand)
		{
			firm2.HiredLabor = Demand;
		}
		else
		{
			firm2.HiredLabor = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumWage2 = 1.0;
		for (double w2 = 0.1; w2 < 100.0; w2 += 0.1)
		{
			Supply = worker.GetLaborSupply2(Price1, Wage1, Wage2, Wage3, Wage4, w2, InterestRate1);
			Demand = firm2.GetLaborDemand(w2, Price3);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumWage2 = w2;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Wage2, EquilibriumWage2,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nWage2 = (1.0 + ConvergenceEquilibrium * PercentageDiff) * Wage2;
		// place money in wage fund
		firm2.CashHolding -= firm2.HiredLabor * Wage2;
		firm2.WageFund += firm2.HiredLabor * Wage2;
		// update quantity vector
		vFirm2HiredLabor[t] = firm2.HiredLabor;
		// sanity checks
		assert(firm2.HiredLabor >= 0.0);
		assert(firm2.WageFund >= 0.0);
		/// END ///

		/// COMMENCE PRODUCTION OF OUTPUT 2 ///
		// commence production
		firm2.SetOutput();

		/// SOLVE MARKET FOR OUTPUT 2 ///
		// find sails
		Supply = firm2.OutputStock;
		Demand = firm1.GetCapitalDemand(Wage1, Price1, Price2);
		if (Supply > Demand)
		{
			firm2.Sails = Demand;
		}
		else
		{
			firm2.Sails = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumPrice2 = 1.0;
		for (double p2 = 0.1; p2 < 100.0; p2 += 0.1)
		{
			Supply = firm2.GetPreferedOutput(Wage2, p2, Price3, InterestRate2);
			Demand = firm1.GetPreferedInput(Wage1, Price1, p2, InterestRate2);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumPrice2 = p2;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Price2, EquilibriumPrice2,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nPrice2 = (1.0 + ConvergenceEquilibrium * PercentageDiff) * Price2;
		// update stock
		firm1.InputStock = firm2.Sails;
		// transfer cash
		firm1.CashHolding -= firm2.Sails * Price2;
		firm2.CashHolding += firm2.Sails * Price1;
		// update quantity vector
		vFirm2Sails[t] = firm2.Sails;
		vFirm2Output[t] = firm2.OutputStock;
		// sanity checks
		assert(firm2.CashHolding >= 0.0);
		assert(firm1.CashHolding >= 0.0);
		assert(firm2.Sails >= 0.0);
		/// END ///

		// SOLVE MARKET FOR LABOR 1 //
		// find labor 1
		Supply = worker.GetLaborSupply1(Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		Demand = firm1.GetLaborDemand(Wage1, Price2);
		if (Supply > Demand)
		{
			firm1.HiredLabor = Demand;
		}
		else
		{
			firm1.HiredLabor = Supply;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumWage1 = 1.0;
		for (double w1 = 0.1; w1 < 100.0; w1 += 0.1)
		{
			Supply = worker.GetLaborSupply1(Price1, w1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
			Demand = firm1.GetLaborDemand(w1, Price2);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumWage1 = w1;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Wage1, EquilibriumWage1,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nWage1 = (1.0 + ConvergenceEquilibrium * PercentageDiff) * Wage1;
		// place money in wage fund
		firm1.CashHolding -= firm1.HiredLabor * Wage1;
		firm1.WageFund += firm1.HiredLabor * Wage1;
		// update quantity vector
		vFirm1HiredLabor[t] = firm1.HiredLabor;
		// sanity checks
		assert(firm1.HiredLabor >= 0.0);
		assert(firm1.WageFund >= 0.0);
		/// END ///

		/// COMMENCE PRODUCTION OF OUTPUT 3 ///
		// commence production
		firm1.SetOutput();

		/// SOLVE MARKET FOR OUTPUT 1 ///
		// find sails
		Supply = firm1.OutputStock;
		double WorkerDemand = worker.GetConsumptionDemand(
			Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1);
		double CapitalistDemand 
			= capitalist.GetConsumptionDemand(Price1, InterestRate1);
		Demand = WorkerDemand + CapitalistDemand;
		if (Supply > Demand)
		{
			firm1.Sails = Demand;
			worker.Consumption = WorkerDemand;
			capitalist.Consumption = CapitalistDemand;
		}
		else
		{
			firm1.Sails = Supply;
			PercentageDiff = (Supply - Demand) / Demand;
			worker.Consumption = (1.0 + PercentageDiff) * WorkerDemand;
			capitalist.Consumption = (1.0 + PercentageDiff) * CapitalistDemand;
		}
		// find equilibrium price
		MinDiff = 1.0;
		double EquilibriumPrice1 = 1.0;
		for (double p1 = 0.1; p1 < 100.0; p1 += 0.1)
		{
			Supply = firm1.GetPreferedOutput(Wage1, p1, Price2, InterestRate2);
			Demand = worker.GetConsumptionDemand(
				Price1, Wage1, Wage2, Wage3, Wage4, Wage5, InterestRate1)
				+ capitalist.GetConsumptionDemand(Price1, InterestRate1);
			double CurrentDiff = Supply - Demand;
			if (abs(CurrentDiff) < MinDiff)
			{
				MinDiff = abs(CurrentDiff);
				EquilibriumPrice1 = p1;
			}
		}
		// find next price
		PercentageDiff = GetPercentageDiff(Price1, EquilibriumPrice1,
			Demand, Supply, ConvergenceEquilibrium, ConvergenceExcess);
		nPrice1 = (1.0 + ConvergenceEquilibrium * PercentageDiff) * Price1;
		// update stock
		firm1.Sails = worker.Consumption + capitalist.Consumption;
		// transfer cash
		worker.CashHolding -= worker.Consumption * Price1;
		capitalist.CashHolding -= capitalist.Consumption * Price1;
		firm1.CashHolding += (worker.Consumption + capitalist.Consumption) * Price1;
		double WorkerMadrassMoney = worker.CashHolding;
		double CapitalistMadrassMoney = capitalist.CashHolding;
		// update quantity vector
		vFirm1Sails[t] = firm1.Sails;
		vFirm1Output[t] = firm1.OutputStock;
		vWorkerConsumption[t] = worker.Consumption;
		vCapitalistConsumption[t] = capitalist.Consumption;
		vWorkerMadrassMoney[t] = WorkerMadrassMoney;
		vCapitalistMadrassMoney[t] = CapitalistMadrassMoney;
		// sanity checks
		assert(firm1.CashHolding >= 0.0);
		assert(worker.CashHolding >= 0.0);
		assert(capitalist.CashHolding >= 0.0);
		assert(firm2.Sails >= 0.0);
		/// END ///

		/// REPAY LOANS, PAY WAGES TO WORKER AND PAY PROFITS TO CAPITALIST ///
		// repay firm loans to bank and get profit
		double Firm1Profit = firm1.CashHolding - firm1.Credit * (1.0 + InterestRate2);
		if (firm1.CashHolding >= firm1.Credit * (1.0 + InterestRate2))
		{
			bank.CashHolding += firm1.Credit * (1.0 + InterestRate2);
			firm1.CashHolding -= firm1.Credit * (1.0 + InterestRate2);
		}
		else
		{
			bank.CashHolding += firm1.CashHolding;
			firm1.CashHolding = 0.0;
		}
		double Firm2Profit = firm2.CashHolding - firm2.Credit * (1.0 + InterestRate2);
		if (firm2.CashHolding >= firm2.Credit * (1.0 + InterestRate2))
		{
			bank.CashHolding += firm2.Credit * (1.0 + InterestRate2);
			firm2.CashHolding -= firm2.Credit * (1.0 + InterestRate2);
		}
		else
		{
			bank.CashHolding += firm2.CashHolding;
			firm2.CashHolding = 0.0;
		}
		double Firm3Profit = firm3.CashHolding - firm3.Credit * (1.0 + InterestRate2);
		if (firm3.CashHolding >= firm3.Credit * (1.0 + InterestRate2))
		{
			bank.CashHolding += firm3.Credit * (1.0 + InterestRate2);
			firm3.CashHolding -= firm3.Credit * (1.0 + InterestRate2);
		}
		else
		{
			bank.CashHolding += firm3.CashHolding;
			firm3.CashHolding = 0.0;
		}
		double Firm4Profit = firm4.CashHolding - firm4.Credit * (1.0 + InterestRate2);
		if (firm4.CashHolding >= firm4.Credit * (1.0 + InterestRate2))
		{
			bank.CashHolding += firm4.Credit * (1.0 + InterestRate2);
			firm4.CashHolding -= firm4.Credit * (1.0 + InterestRate2);
		}
		else
		{
			bank.CashHolding += firm4.CashHolding;
			firm4.CashHolding = 0.0;
		}
		double Firm5Profit = firm5.CashHolding - firm5.Credit * (1.0 + InterestRate2);
		if (firm5.CashHolding >= firm5.Credit * (1.0 + InterestRate2))
		{
			bank.CashHolding += firm5.Credit * (1.0 + InterestRate2);
			firm5.CashHolding -= firm5.Credit * (1.0 + InterestRate2);
		}
		else
		{
			bank.CashHolding += firm5.CashHolding;
			firm5.CashHolding = 0.0;
		}
		// repay bank loans to worker and capitalist and get profit
		double BankProfit = bank.CashHolding
			- (worker.Investment + capitalist.Investment) * (1.0 + InterestRate1);
		if (bank.CashHolding
			>= (worker.Investment + capitalist.Investment) * (1.0 + InterestRate1))
		{
			worker.CashHolding += worker.Investment * (1.0 + InterestRate1);
			capitalist.CashHolding += capitalist.Investment * (1.0 + InterestRate1);
			bank.CashHolding -=
				(worker.Investment + capitalist.Investment) * (1.0 + InterestRate1);
		}
		else
		{
			double WorkerFraction = worker.Investment 
				/ (worker.Investment + capitalist.Investment);
			worker.CashHolding += bank.CashHolding * WorkerFraction;
			capitalist.CashHolding += bank.CashHolding * (1.0 - WorkerFraction);
			bank.CashHolding = 0.0;
		}
		// repay profits to capitalist
		capitalist.CashHolding += firm1.CashHolding;
		capitalist.CashHolding += firm2.CashHolding;
		capitalist.CashHolding += firm3.CashHolding;
		capitalist.CashHolding += firm4.CashHolding;
		capitalist.CashHolding += firm5.CashHolding;
		capitalist.CashHolding += bank.CashHolding;
		firm1.CashHolding = 0.0;
		firm2.CashHolding = 0.0;
		firm3.CashHolding = 0.0;
		firm4.CashHolding = 0.0;
		firm5.CashHolding = 0.0;
		bank.CashHolding = 0.0;
		// pay wages to worker
		double TotalWagePayments = 0.0;
		worker.CashHolding += firm1.WageFund;
		worker.CashHolding += firm2.WageFund;
		worker.CashHolding += firm3.WageFund;
		worker.CashHolding += firm4.WageFund;
		worker.CashHolding += firm5.WageFund;
		TotalWagePayments += firm1.WageFund;
		TotalWagePayments += firm2.WageFund;
		TotalWagePayments += firm3.WageFund;
		TotalWagePayments += firm4.WageFund;
		TotalWagePayments += firm5.WageFund;
		firm1.WageFund = 0.0;
		firm2.WageFund = 0.0;
		firm3.WageFund = 0.0;
		firm4.WageFund = 0.0;
		firm5.WageFund = 0.0;
		// update quantity vectors
		vFirm1Profit[t] = Firm1Profit;
		vFirm2Profit[t] = Firm2Profit;
		vFirm3Profit[t] = Firm3Profit;
		vFirm4Profit[t] = Firm4Profit;
		vFirm5Profit[t] = Firm5Profit;
		vBankProfit[t] = BankProfit;
		vTotalProfits[t] = BankProfit + Firm1Profit + Firm2Profit
			+ Firm3Profit + Firm4Profit + Firm5Profit;
		vTotalWagePayments[t] = TotalWagePayments;
		// sanity checks
		assert(worker.CashHolding >= 0.0);
		assert(capitalist.CashHolding >= 0.0);
		/// END ///

		/// SANITY CHECKS
		// just a whole lot of addtional sanity checks
		assert(firm1.HiredLabor >= 0.0);
		assert(firm2.HiredLabor >= 0.0);
		assert(firm3.HiredLabor >= 0.0);
		assert(firm4.HiredLabor >= 0.0);
		assert(firm5.HiredLabor >= 0.0);
		assert(firm1.OutputStock >= 0.0);
		assert(firm2.OutputStock >= 0.0);
		assert(firm3.OutputStock >= 0.0);
		assert(firm4.OutputStock >= 0.0);
		assert(firm5.OutputStock >= 0.0);
		assert(worker.Investment >= 0.0);
		assert(capitalist.Investment >= 0.0);
		assert(firm1.Credit >= 0.0);
		assert(firm2.Credit >= 0.0);
		assert(firm3.Credit >= 0.0);
		assert(firm4.Credit >= 0.0);
		assert(firm5.Credit >= 0.0);
		assert(WorkerMadrassMoney >= 0.0);
		assert(CapitalistMadrassMoney >= 0.0);
		/// END ///

		/// OUTPUT PERIOD DATA ///
		std::cout << "PERIOD " << t << " DATA READY" << std::endl;
		char MainChoice;
		std::cout << "Display Data? (y/n)" << std::endl;
		std::cin >> MainChoice;
		if (MainChoice == 'y')
		{
			char Choice1;
			std::cout << "Display Sanity Checks? (y/n)" << std::endl;
			std::cin >> Choice1;
			if (Choice1 == 'y')
			{
				std::cout << "Total Cash Holding: " <<
					worker.CashHolding + capitalist.CashHolding + bank.CashHolding
					+ firm1.CashHolding + firm2.CashHolding + firm3.CashHolding
					+ firm4.CashHolding + firm5.CashHolding << std::endl;
				std::cout << "Worker Cash Holding: " << worker.CashHolding << std::endl;
				std::cout << "Capitalist Cash Holding: " << capitalist.CashHolding << std::endl;
				std::cout << "Bank Cash Holding: " << bank.CashHolding << std::endl;
				std::cout << "Firm1 Cash Holding: " << firm1.CashHolding << std::endl;
				std::cout << "Firm2 Cash Holding: " << firm2.CashHolding << std::endl;
				std::cout << "Firm3 Cash Holding: " << firm3.CashHolding << std::endl;
				std::cout << "Firm4 Cash Holding: " << firm4.CashHolding << std::endl;
				std::cout << "Firm5 Cash Holding: " << firm5.CashHolding << std::endl;
			}
			char Choice2;
			std::cout << "Display Prices? (y/n)" << std::endl;
			std::cin >> Choice2;
			if (Choice2 == 'y')
			{
				std::cout << "Price / Next Price / Equilibrium Price" << std::endl;
				std::cout << "P1: " << Price1 << "\t " << nPrice1 << "\t " << EquilibriumPrice1 << std::endl;
				std::cout << "P2: " << Price2 << "\t " << nPrice2 << "\t " << EquilibriumPrice2 << std::endl;
				std::cout << "P3: " << Price3 << "\t " << nPrice3 << "\t " << EquilibriumPrice3 << std::endl;
				std::cout << "P4: " << Price4 << "\t " << nPrice4 << "\t " << EquilibriumPrice4 << std::endl;
				std::cout << "P5: " << Price5 << "\t " << nPrice5 << "\t " << EquilibriumPrice5 << std::endl;
				std::cout << "W1: " << Wage1 << "\t " << nWage1 << "\t " << EquilibriumWage1 << std::endl;
				std::cout << "W2: " << Wage2 << "\t " << nWage2 << "\t " << EquilibriumWage2 << std::endl;
				std::cout << "W3: " << Wage3 << "\t " << nWage3 << "\t " << EquilibriumWage3 << std::endl;
				std::cout << "W4: " << Wage4 << "\t " << nWage4 << "\t " << EquilibriumWage4 << std::endl;
				std::cout << "W5: " << Wage5 << "\t " << nWage5 << "\t " << EquilibriumWage5 << std::endl;
				std::cout << "R1: " << InterestRate1 << "\t " << nInterestRate1 << "\t " << EquilibriumInterestRate1 << std::endl;
				std::cout << "R2: " << InterestRate2 << "\t " << nInterestRate2 << "\t " << EquilibriumInterestRate2 << std::endl;
			}
			char Choice3;
			std::cout << "Display Quantities? (y/n)" << std::endl;
			std::cin >> Choice3;
			if (Choice3 == 'y')
			{
				std::cout << "Wage Income: " << TotalWagePayments << std::endl;
				std::cout << "Worker Consumption: " << worker.Consumption << std::endl;
				std::cout << "Capitalist Consumption: " << capitalist.Consumption << std::endl;
				std::cout << "Worker Madrass Money: " << WorkerMadrassMoney << std::endl;
				std::cout << "Capitalist Madrass Money: " << CapitalistMadrassMoney << std::endl;
				std::cout << "Firm1 Sails: " << firm1.Sails << std::endl;
				std::cout << "Firm2 Sails: " << firm2.Sails << std::endl;
				std::cout << "Firm3 Sails: " << firm3.Sails << std::endl;
				std::cout << "Firm4 Sails: " << firm4.Sails << std::endl;
				std::cout << "Firm5 Sails: " << firm5.Sails << std::endl;
				std::cout << "Firm1 Labor: " << firm1.HiredLabor << std::endl;
				std::cout << "Firm2 Labor: " << firm2.HiredLabor << std::endl;
				std::cout << "Firm3 Labor: " << firm3.HiredLabor << std::endl;
				std::cout << "Firm4 Labor: " << firm4.HiredLabor << std::endl;
				std::cout << "Firm5 Labor: " << firm5.HiredLabor << std::endl;
			}
			char Choice4;
			std::cout << "Display Profits? (y/n)" << std::endl;
			std::cin >> Choice4;
			if (Choice4 == 'y')
			{
				std::cout << "Bank Profit: " << BankProfit << std::endl;
				std::cout << "Firm1 Profit: " << Firm1Profit << std::endl;
				std::cout << "Firm2 Profit: " << Firm2Profit << std::endl;
				std::cout << "Firm3 Profit: " << Firm3Profit << std::endl;
				std::cout << "Firm4 Profit: " << Firm4Profit << std::endl;
				std::cout << "Firm5 Profit: " << Firm5Profit << std::endl;
			}
		}
		std::cout << "-----------------------------------------" << std::endl;
		/// END ///

				/// SET PRICES TO NEXT PRICES ///
		Price1 = nPrice1;
		Price2 = nPrice2;
		Price3 = nPrice3;
		Price4 = nPrice4;
		Price5 = nPrice5;
		Wage1 = nWage1;
		Wage2 = nWage2;
		Wage3 = nWage3;
		Wage4 = nWage4;
		Wage5 = nWage5;
		InterestRate1 = nInterestRate1;
		InterestRate2 = nInterestRate2;
		/// END ///
	}

	/// WRITE DATA TO FILE ///
	std::ofstream ost{ "Data.txt" };
	ost << "Period " << "t: \t P1 \t P2 \t P3 \t P4 \t P5 \t W1 \t W2 \t W3 \t W4 \t W5 \t R1 \t R2" << std::endl;
	ost.close();
	/// END ///
}

