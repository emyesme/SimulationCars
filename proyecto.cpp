#include <iostream>
#include <random>
#include <vector>
#include <sstream>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

struct Car {
	double timeForFail;
	double cost;
	
	Car(int m, exponential_distribution<double> &distributionIn, default_random_engine &generatorIn){
		
		this->cost = 5+4*m+0.2*m*m;
		this->timeForFail = this->repairCost(distributionIn, generatorIn);
		
		if(this->timeForFail <= 4){
			this->cost += 6.67;
		}
	}
	
	double repairCost(exponential_distribution<double> &distributionIn, default_random_engine &generatorIn){	
		return distributionIn(generatorIn);
	}
};

void carsGenerator(int m,double &sum,int start, int years,int &failCars, double &maxtime, double &mintime, vector<Car> &myCars, vector<double> &yline);
void carsTimeDamage(int m, int years, vector<Car> myCars, vector<double> &yline);
void carsHistogramDamage(int maxtime, int mintime, int widthInterval, vector<Car> myCars);
void carsFailGood(int m, int start,int years,vector <Car> &myCars,vector <double> &yline, vector <int> &compare);
void carsCostInterval(int m1, int m2, int start,int years,vector <Car> &myCars,vector <double> &yline, vector <int> &compare);


int main(){
	double sum = 0;
	int failCars = 0;
	double maxtime = 0;
	double mintime = 9999;
	int years = 100;
	
	//cout << "el costo inicial es "<< 5+4*m+0.2*m*m << endl;

	vector<Car> myCars;
	vector<double> yline (years*10);
	vector <int> compare;
	
	carsGenerator(4,sum,0,100,failCars,maxtime,mintime,myCars,yline);
	/*Grafica de carros versus averia en años */
	//carsTimeDamage(m, years, myCars, yline);

	/*Grafica de tabla de frecuencias */
	//carsHistogramDamage(maxtime, mintime, 1, myCars);
	
	/*total vs fail 1000-2000 m=4 */
	//carsFailGood(4, 1000, 200, myCars, yline, compare);

	/*total vs fail 1000-2000 6 */
	//carsFailGood(6, 1000, 200, myCars, yline, compare);

	/*Grafica cost m=4 y cost m = 6 0-1000 */
	//carsCostInterval(4,6,0,100,myCars,yline,compare);

	/*Grafica cost m=4 y cost m = 6 1000-2000 */
	//carsCostInterval(4,6,1000,200,myCars,yline,compare);

	/*Grafica cost m=4 y cost m = 6 2000-3000 */
	carsCostInterval(4,6,2000,300,myCars,yline,compare);

	return 0;
}

void carsGenerator(int m,double &sum,int start, int years,int &failCars, double &maxtime, double &mintime, vector<Car> &myCars, vector<double> &yline){
	Car * currentCar = NULL;
	double lambda = (double) 1/m;
	//Generador de números aleatorios
	default_random_engine generator;
	//Distribución exponencial
	exponential_distribution<double> distribution(lambda);
	int cars = 0;
	int valid = 0;
	for(int i=1; i<=years; i++){
		for (int j=1; j<=10; j++){
			if(cars < start){
				currentCar = new Car(m, distribution, generator);
				currentCar->timeForFail = 0;
				currentCar->cost = 0;
				myCars.insert(myCars.begin(),*currentCar);
			}else{
				valid++;
				currentCar = new Car(m, distribution, generator);
				myCars.insert(myCars.begin(),*currentCar);
				if(currentCar->timeForFail <= 4){
					++failCars;
				}
				//cout << "\nIndex " << (i-1)*10+j << endl;
				//cout << "Tiempo " << a->timeForFail << endl;
				if(maxtime < myCars.begin()->timeForFail){
					maxtime = myCars.begin()->timeForFail;
				}
				if(mintime > myCars.begin()->timeForFail){
					mintime = myCars.begin()->timeForFail;
				}
				//The index of actual car
				sum = (sum * (valid-1) + currentCar->cost)/valid;
			}
			cars++;
		}
	}
}

void carsTimeDamage(int m, int years, vector<Car> myCars, vector<double> &yline){
	vector <double> myTime (years*10);
	for(vector<Car>::iterator it=myCars.begin(); it != myCars.end(); it++){
		myTime.insert(myTime.begin(),it->timeForFail);
	}
	fill(yline.begin(),yline.end(), m);
	plt::xlim(0,years*10);
	plt::plot(myTime,"*");
	plt::named_plot("garantia",yline,"r--");
	plt::legend();
	plt::ylabel("averia (anios)");
	plt::xlabel("Autos generados");
	plt::title("Generando fallas");
	plt::show();
	plt::clf();
}

void carsHistogramDamage(int maxtime, int mintime, int widthInterval, vector<Car> myCars){
	int diffTime = maxtime - mintime;
	int numIntervals = diffTime / widthInterval;
	int freqIntervals [numIntervals];
	fill(freqIntervals, freqIntervals+numIntervals,0);
	//counting
	for(vector<Car>::iterator it=myCars.begin(); it != myCars.end(); it++){
		for(int j = 0; j<numIntervals; j++){
			if(it->timeForFail >= j && it->timeForFail < j+1){
				freqIntervals[j] = freqIntervals[j] + 1;
			}
		}
	}
	vector <int> histogram(freqIntervals,freqIntervals+numIntervals);
	plt::xlim(0,numIntervals);
	plt::bar(histogram);
	plt::show();
	plt::clf();
}

void carsCostInterval(int m1, int m2, int start,int years,vector <Car> &myCars,vector <double> &yline, vector <int> &compare){
	double sum = 0;
	int failCars = 0;
	double maxtime = 0;
	double mintime = 9999;
	myCars.clear();
	yline.clear();
	compare.clear();

	carsGenerator(m1,sum,start,years,failCars,maxtime,mintime,myCars,yline);
	cout << "m " << m1 <<" EL COSTO PROMEDIO POR CARRO ES: " << sum << endl;
	compare.push_back(sum);

	sum = 0;
	failCars = 0;
	maxtime = 0;
	mintime = 9999;
	myCars.clear();
	yline.clear();

	carsGenerator(m2,sum,start,years,failCars,maxtime,mintime,myCars,yline);
	cout << "m " << m2 << " EL COSTO PROMEDIO POR CARRO ES: " << sum << endl;	
	compare.push_back(sum);

	vector <string> labels (2);
	vector <int> ticks (2);

	labels.insert(labels.begin(),"m=4");
	labels.insert(labels.begin(),"m=6");

	ticks.insert(ticks.begin(),0);
	ticks.insert(ticks.begin(),1);

	plt::clf();
	plt::bar(compare);
	plt::xticks(ticks,labels);
	plt::title("Costo");
	plt::show();
	plt::clf();

}

void carsFailGood(int m, int start,int years,vector <Car> &myCars,vector <double> &yline, vector <int> &compare){
	double sum = 0;
	int failCars = 0;
	double maxtime = 0;
	double mintime = 9999;
	int versus [2] = {0,0};
	myCars.clear();
	yline.clear();
	compare.clear();

	carsGenerator(m,sum,start,years,failCars,maxtime,mintime,myCars,yline);	

	versus[0] = years*10 - start - failCars;
	versus[1] = failCars;
	compare.insert(compare.begin(), begin(versus),end(versus));

	plt::clf();
	plt::bar(compare);
	plt::title("sin Fallos vs con Fallos");
	std::string out_1, out_2, out_3;
	std::stringstream ss1, ss2, ss3;
	ss1 << years*10;
	ss2 << m;
	ss3 << start;
	out_1 = ss1.str();
	out_2 = ss2.str();
	out_3 = ss3.str();
	plt::save(out_3+"-"+out_1+"m"+out_2+".png");
	plt::clf();
}