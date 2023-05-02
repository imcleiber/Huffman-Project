library(readr)
library(knitr)

dadosplot <- read_csv("contagem.csv")

AVL <- dadosplot$AVL
AD <- dadosplot$AD
y <- dadosplot$N
n <- 1000
##-----------------------------AVL----------------------------------------------

plot(y, AVL, pch = 20, col = "black", 
     ylab = "Quantidade de Testes", 
     xlab = "Ordem de Inserção Aleatoria",
     xlim = c(0,1000),
     ylim = c(0,30),
     main = "AVL")

points(y, AD, col = "gray", pch = 20)

##-----------------------Arvore Desbalanceada-----------------------------------


plot(y, AD, pch = 20, col = "red", 
     ylab = "Quantidade de Testes", 
     xlab = "Ordem de Inserção Aleatoria",
     xlim = c(0,1000),
     ylim = c(0,35),
     main = "Arvore Desbalanceada")

points(y, AVL, col = "black", pch = 20)

