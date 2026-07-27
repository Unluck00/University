


# EPS A 29 nov 24


library(UsingR)
data("stud.recs")

t.test(stud.recs$sat.m, alternative = "two.sided", mu= 500)

data(OBP)

mean(OBP)
t.test(OBP, alternative = "two.sided", mu = 0.33)
t.test(OBP, alternative = "two.sided", mu = 0.33) -> test

test$p.value

data("normtemp")
normtemp$tempC <- (normtemp$temperature -32)/1.8

valore1<-(98.6 -32)/1.8
valore2<-(98.2 -32)/1.8

t.test(normtemp$tempC, alternative = "less", mu = valore1)


binom.test(40, 50, alternative = "greater", p = 0.75)-> ptest
binom.test(400, 500, alternative = "greater", p = 0.75)


2700/25000

binom.test(2700, 25000, alternative = "greater", p = 0.1)

data(babies)

t.test(babies$dage, babies$age, alternative = "greater", mu = 0, paired = TRUE)

install.packages("HistData")
library(HistData)
data(Galton)

Galton$parentCM <- Galton$parent * 2.54
Galton$childCM <- Galton$child * 2.54

boxplot(Galton$parentCM, Galton$childCM)

t.test(Galton$parentCM, Galton$childCM, alternative = "two.sided", mu = 0, paired = TRUE)
       
t.test(Galton$parentCM, Galton$childCM, alternative = "greater", mu = 0, paired = TRUE)

       
       
       
       

