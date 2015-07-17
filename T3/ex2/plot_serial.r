# plot graphics for ex2
library(ggplot2)

x <- c('small', 'medium', 'large')
y <- c(9.21e-05, 0.0018475, 0.0081566)

png(file="ex2_serial.png", bg="transparent")

ggplot() + geom_point(aes(x = x, y = y)) + scale_size_area() + xlab("Tamanho da matriz") + ylab("Tempo (s)") + ggtitle("Produto de Matrizes (versão serial)")

dev.off()
