
# r EPSA 13 nov 24

data(rivers) #carica il dataset rivers, che è incluso nel pacchetto stesso

rivers_km <- rivers*1.60934  #converte miglia (unità di misura standar per il pacchetto rivers) in km
soglia <- 500*1.60934  #converte miglia in km di soglia

# numero totale di fiumi con lunghezza < soglia
sum(rivers_km < soglia)  #con sum restituisce quanti sono i true della condizione

sum(rivers_km < soglia)/141
sum(rivers_km < soglia)/length(rivers_km)

media <- mean(rivers_km)  #media dei km di rivers
sum(rivers_km < media)/length(rivers_km)


quantile(rivers_km,0.75)  #calcola il 0.75 quantile, 75°-percentile campionario

quantile(rivers_km, c(0.25,0.5,0.75))  #con c restituisce i 3 quartini

options(digits = 17)  #cambia i numeri decimali
quantile(rivers_km,0.75)

library(UsingR)
data(nym.2002)

str(nym.2002)  #fa vedere una struttura compatta (non piu un vettore, ma un dataframe)

# trasformo le tre ore in minuti
soglia <- 180  

nym.2002$time  #per accedere alla colonna time (rimane dataframe (1000 rige e una colonna))
str(nym.2002$time)  #convertito in vettore

sum(nym.2002$time < soglia)/length(nym.2002$time)  #nym.2002$ restiuisce le variabili non i valori
quantile(nym.2002$time, 0.1) 
quantile(nym.2002$time, 0.25)
quantile(nym.2002$time, 0.9)

########## #variabili qualitative

table(nym.2002$gender)  #fa la tabella di frequenza della colonna gender

barplot(table(nym.2002$gender))  #fa il grafico del barplot (con il parametro che indica l'altezza)
barplot(table(nym.2002$gender), horiz = TRUE) #legge il grafico in orizzontale

dotchart(table(nym.2002$gender)) #fa il grafico del dotplot

barplot(table(nym.2002$home))
max(table(nym.2002$home))  #visualizza solo il valore massimo della tabella
str(table(nym.2002$home)) ##solo per provare non serve a niente

which.max(table(nym.2002$home))  #da la posizione (indice del vettore) del massimo nella tabella


data(babies)
str(babies)
str(babies$smoke)

babies$smoke[babies$smoke == 9] <- NA #sostituiamo i 9 con i NA (dato mancante); [babies$smoke == 9] applica un filtro, selezionando solo gli elementi della colonna smoke che soddisfano la condizione babies$smoke == 9 (cioè i valori che sono uguali a 9) 
smoke_factor <- as.factor(babies$smoke) 
levels(smoke_factor)
levels(smoke_factor) <- c("never", "now", "until_p", "once")
levels(smoke_factor)
is.na(smoke_factor)

barplot(table(smoke_factor))

