package col

import (
	"math"
)

// Given a sequence of integers X and input y, find index i that minimizes
// abs(X[i] - y).

type Strings struct {
	idx map[int]string
	lst []int
}

func (st *Strings) Insert(s string, n int) {
	if st.idx == nil {
		st.idx = make(map[int]string)
		st.lst = make([]int, 0)
	}
	st.idx[len(st.lst)] = s
	st.lst = append(st.lst, n)
}

func (st *Strings) NearestTo(n int) (s string) {
	if len(st.lst) == 0 {
		return ""
	}
	i := 0
	m := st.lst[0]
	for j, x := range st.lst {
		if math.Abs(float64(x-n)) < math.Abs(float64(m-n)) {
			i = j
			m = x
		}
	}
	return st.idx[i]
}
