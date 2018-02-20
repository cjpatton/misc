package col

import (
	"math"
)

// Given a sequence of integers X and input y, find index i that minimizes
// abs(X[i] - y).

type Strings struct {
	idx      map[int]string
	lst      []float64
	unsorted int
}

func (st *Strings) Insert(s string, n float64) {
	if st.idx == nil {
		st.idx = make(map[int]string)
		st.lst = make([]float64, 0)
		st.unsorted = 0
	}
	st.idx[len(st.lst)] = s
	st.lst = append(st.lst, n)
	st.unsorted++
}

func (st *Strings) NearestTo(n float64) (s string) {
	if len(st.lst) == 0 {
		return ""
	}
	if st.unsorted > 0 {
		st.isort(st.unsorted)
		st.unsorted = 0
	}

	i := bsearch(st.lst, 0, len(st.lst)-1, n)
	return st.idx[i]
}

func bsearch(X []float64, i, j int, n float64) int {
	if j == i {
		return i
	} else if j-i == 1 {
		if math.Abs(n-X[i]) < math.Abs(n-X[j]) {
			return i
		}
		return j
	}

	p := (j + i) >> 1
	a := math.Abs(n - X[i])
	b := math.Abs(n - X[p])
	c := math.Abs(n - X[j])

	if n < X[p] {
		if a < b { // closer to i than p
			return bsearch(X, i, p-1, n)
		}
		return bsearch(X, i+1, p, n)
	} else if n > X[p] {
		if c < b { // closer to j than p
			return bsearch(X, p+1, j, n)
		}
		return bsearch(X, p, j-1, n)
	}
	return p // p is a closest point
}

func (st *Strings) linearNearestTo(n float64) string {
	if len(st.lst) == 0 {
		return ""
	}
	i := 0
	m := st.lst[0]
	for j, x := range st.lst {
		if math.Abs(x-n) < math.Abs(m-n) {
			i = j
			m = x
		}
	}
	return st.idx[i]
}

func (st *Strings) isort(ct int) {
	for i := len(st.lst) - 1; i >= len(st.lst)-ct; i-- {
		for j := i - 1; j >= 0; j-- {
			if st.lst[j] > st.lst[i] {
				st.swap(i, j)
			}
		}
	}
}

func (st *Strings) swap(i, j int) {
	n := st.lst[i]
	st.lst[i] = st.lst[j]
	st.lst[j] = n
	s := st.idx[i]
	st.idx[i] = st.idx[j]
	st.idx[j] = s
}
