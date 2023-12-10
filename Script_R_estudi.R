library(googlesheets4)
gs4_auth(email <- "alex.rodriguez.r@estudiantat.upc.edu")
my_data_frame <- read_sheet("1jm5mNztwoCVIQri8rp04wK9kd86539d_If4L0HcWY-0")

DFS <- log(as.numeric(my_data_frame$`Temps DFS(micros)`))
A_star <- log(as.numeric(my_data_frame$`Temps A*(micros)`))

D <- DFS - A_star
n <- my_data_frame$`Tamany del laberint`
h <- as.numeric(my_data_frame$`Distància de Manhattan`)
b <- as.numeric(my_data_frame$`Avg. paths/cell`)

model <- lm(D ~ n + h + b)
summary(model)

IC <- confint(model, level = 0.95)
IC

plot(model, c(2,3))
hist(rstandard(model))
plot(rstandard(model), type = "l")
plot(fitted(model), residuals(model)) 
abline(h = 0, col = "red", lty = 2)



