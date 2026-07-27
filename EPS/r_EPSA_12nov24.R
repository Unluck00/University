# eps A 12 nov 24

install.packages("UsingR")  //installiamo la libreria
  //il pacchetto UsingR contiene vari dati di esempio, utili per esercizi statistici e dimostrazioni

library(UsingR)  //carichiamo la libreria
data("bumpers")  //carica il dataset bumpers, che è incluso nel pacchetto stesso

hist(bumpers)  //istogramma di bumpers
hist(bumpers, breaks = 20)

pnorm(-5,10,5)
1-pnorm(25,10,5)

pnorm(-1,2,1)

pnorm(25,10,5) - pnorm(-5,10,5)
(max(bumpers)-min(bumpers))/6


mean(bumpers)
median(bumpers)
sd(bumpers)


data(firstchi)
hist(firstchi)
hist(firstchi, breaks = 15)


mean(firstchi)
median(firstchi)
sd(firstchi)
