replace x y [] = []
replace x y (z:zs)
	|z == x = y:replace x y zs
	|otherwise = z:replace x y zs
	
	
	
delete k [] = []
delete k lt
	|(length lt) > k = append (take (k-1) lt)(delete k (drop k lt))
	|otherwise = lt
	
	
maxodd lt
	|(length lt)>2 && (head lt)>(head(tail(tail lt))) = maxodd((head lt):(tail(tail(tail lt))))
	|(length lt)>2 && (head lt)<=(head(tail(tail lt))) = maxodd(tail(tail lt))
	|otherwise = head lt
	
union [] [] = []
union lt1 lt2 = deletesame(append lt1 lt2)

append [] ys = ys
append (x:xs) ys = x:append xs ys

alldelete x [] = []
alldelete x (y:ys)
	|x == y = alldelete x ys
	|otherwise = y:alldelete x ys

deletesame [] = []
deletesame(x:xs) = x:(deletesame(alldelete x xs))
