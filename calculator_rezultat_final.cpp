#include <iostream>
#include <cmath>

int main()
{
    //Declararea variabilelor si introducerea valorilor de la tastatura

    unsigned int nOfRuns;
    float meanDose, meanDoseErr;

    std::cout << "Introduceti numarul de Run-uri: ";
    std::cin >> nOfRuns;

    float dose[nOfRuns], doseErr[nOfRuns];  //array-uri cu dimensiunea nOfRuns pt. rezultatele dupa fiecare Run

    std::cout << "********** Introducerea rezultatelor din fiecare Run **********" << std::endl;
    for(unsigned int i=0; i<nOfRuns; i++)
    {
        std::cout << "dose_" << i << " = ";
        std::cin >> dose[i];  //umplerea lui dose[i]

        std::cout << "doseError_" << i << " = ";
        std::cin >> doseErr[i];  //umplerea lui doseErr[i]
    }
    std::cout << "***************************************************************" << std::endl;



    //Calcularea valorii finale a dozei medii si erorea ei

    float sumDose = 0, sumErr = 0;  //numaratorul si numitorul din formula lui x_mediu

    for(unsigned int i=0; i<nOfRuns; i++)
    {
        sumDose += dose[i] / std::pow(doseErr[i], 2);  //numaratorul
        sumErr += 1 / std::pow(doseErr[i], 2);  //numitorul
    }

    meanDose = sumDose / sumErr;
    meanDoseErr = 1 / std::sqrt(sumErr);

    std::cout << "-> Doza medie = " << meanDose << std::endl;
    std::cout << "-> Eroarea = +/- " << meanDoseErr << std::endl;

    return 0;
}
