
# EPSA 8 nov 24

(dpois(4,1)*0.4)/(dpois(4,3)*0.6+dpois(4,1)*0.4)

0.09+0.05+0.07+0.11

1-0.09/0.32

imx <- c(0,1,2,3)
pmfx <- c(0.32, 0.28, 0.18, 0.22)

sum(imx*pmfx) -> mediax

(imx - mediax)^2 -> scarti2
sum(scarti2*pmfx)
