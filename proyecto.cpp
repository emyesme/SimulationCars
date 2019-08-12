#include <iostream>
#include <random>
#include <vector>
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

void carsGenerator(int m,double &sum,int start, int years,int &failCars, double &maxtime, double &mintime, vector<Car> &myCars, vector<double> &yline){
	Car * currentCar = NULL;
	cout << m << endl;
	double lambda = (double) 1/m;
	//Generador de números aleatorios
	default_random_engine generator;
	//Distribución exponencial
	exponential_distribution<double> distribution(lambda);
	int cars = 0;
	int trash = 0;
	for(int i=1; i<=years; i++){
		for (int j=1; j<=10; j++){
			if(cars < start){
				trash++;
				currentCar = new Car(m, distribution, generator);
				currentCar->timeForFail = 0;
				currentCar->cost = 0;
				myCars.insert(myCars.begin(),*currentCar);
			}else{
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
				int nCar = (i-1)*10+j;
				sum = (sum * (nCar-1) + currentCar->cost)/nCar;
			}
			cars++;
		}
	}
	cout << "\ncars " << cars << endl;
	cout << "trash "  << trash << endl;
}



int main(){
	double sum = 0;
	int start = 0;
	int widthInterval = 1;
	double maxtime = 0;
	double mintime = 9999;	
	int years = 100;
	int m = 4;
	int failCars = 0;
	cout << "el costo inicial es "<< 5+4*m+0.2*m*m << endl;
	vector<Car> myCars;
	vector<double> yline (years*10);
	vector <int> compare (2);
	int versus [2] = {0,0};
	//////////////////////////////////////////////////////
	carsGenerator(m,sum,start,years,failCars,maxtime,mintime,myCars,yline);
	//////////////////////////////////////////////////////
	cout << "EL COSTO PROMEDIO POR CARRO ES: " << sum << endl;
	cout << "El número de carros dañados fue: " << failCars << endl;
	//tiempo en que se dañaron los autos con linea de garantia
	/*
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
 	*/
	//frecuencias acumuladas para ver exponencial
	/*
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
	*/

	//tabla de frecuencias
	/*
	cout << "frecuencias" << endl;
	cout << "intervalos: " << numIntervals << endl;
	for(int j = 0; j<numIntervals; j++){
		cout << j << ","<< j+1 <<" | "<< freqIntervals[j] << endl;
	}*/

	//total vs fail 0-1000
	
	versus[0] = years*10 - start - failCars;
	versus[1] = failCars;
	compare.insert(compare.begin(), begin(versus),end(versus));
	plt::clf();
	plt::bar(compare);
	plt::title("sin Fallos vs con Fallos m=4");
	//plt::save("0-1000m4.png");
	plt::clf();
	
	
	//total vs fail 1000-2000 m=4
	sum = 0;
	start = 1000;
	widthInterval = 1;
	maxtime = 0;
	mintime = 9999;	
	years = 200;
	m = 4;
	failCars = 0;
	myCars.clear();
	yline.clear();
	compare.clear();
	//////////////////////////////////////////////////////
	carsGenerator(m,sum,start,years,failCars,maxtime,mintime,myCars,yline);
	//////////////////////////////////////////////////////
	cout << "EL COSTO PROMEDIO POR CARRO ES: " << sum << endl;
	cout << "El número de carros dañados fue: " << failCars << endl;
	versus[0] = years*10 - start - failCars;
	versus[1] = failCars;
	compare.insert(compare.begin(), begin(versus),end(versus));
	cout << "failcars " << failCars << endl;
	plt::bar(compare);
	plt::title("sin Fallos vs con Fallos m=4");
	//plt::save("1000-2000m4.png");
	plt::clf();

	//total vs fail 0-1000 6
	sum = 0;
	start = 0;
	widthInterval = 1;
	maxtime = 0;
	mintime = 9999;	
	years = 100;
	m = 6;
	failCars = 0;
	myCars.clear();
	yline.clear();
	compare.clear();
	//////////////////////////////////////////////////////
	carsGenerator(m,sum,start,years,failCars,maxtime,mintime,myCars,yline);
	//////////////////////////////////////////////////////
	cout << "EL COSTO PROMEDIO POR CARRO ES: " << sum << endl;
	cout << "El número de carros dañados fue: " << failCars << endl;
	versus[0] = years*10 - start - failCars;
	versus[1] = failCars;
	compare.insert(compare.begin(), begin(versus),end(versus));
	cout << "failcars " << failCars << endl;
	plt::clf();
	plt::bar(compare);
	plt::title("sin Fallos vs con Fallos m=6");
	//plt::save("0-1000m6.png");
 	plt::clf();


	//total vs fail 1000-2000 6
	sum = 0;
	start = 1000;
	widthInterval = 1;
	maxtime = 0;
	mintime = 9999;	
	years = 200;
	m = 6;
	failCars = 0;
	myCars.clear();
	yline.clear();
	compare.clear();
	//////////////////////////////////////////////////////
	carsGenerator(m,sum,start,years,failCars,maxtime,mintime,myCars,yline);
	//////////////////////////////////////////////////////
	cout << "EL COSTO PROMEDIO POR CARRO ES: " << sum << endl;
	cout << "El número de carros dañados fue: " << failCars << endl;
	versus[0] = years*10 - start - failCars;
	versus[1] = failCars;
	compare.insert(compare.begin(), begin(versus),end(versus));
	cout << "failcars " << failCars << endl;
	plt::clf();
	plt::bar(compare);
	plt::title("sin Fallos vs con Fallos m=6");
	//plt::save("1000-2000m6.png");
 	plt::clf();

	
	return 0;
}
