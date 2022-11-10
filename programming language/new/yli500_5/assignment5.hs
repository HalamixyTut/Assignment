insert_even x [] = []
insert_even x [y] = [y]
insert_even x (z:m:zs) = z:m:x:insert_even x zs


rmlt [] = []
rmlt (x:xs)
	|x>=5 = x:rmlt xs
	|otherwise = rmlt xs
	

addgt x [] = []
addgt x (y:ys)
	|y>2 = (y+x):addgt x ys
	|otherwise = y:addgt x ys
	
comp n lt = drop (n-1) lt


addfirst 0 xs = xs
addfirst n lt = 4:addfirst (n-1) lt

firstn n lt = take n lt

num_occur x [] = 0
num_occur x (y:ys)
	|x==y = 1+num_occur x ys
	|otherwise = num_occur x ys
	
	
incn lt 0 = lt
incn (x:xs) n
	|n>=1 = (x+1):incn xs (n-1)
	|otherwise = x:xs
