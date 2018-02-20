package col

import (
	"math"
)

// Given a sequence of integers X and input y, find index i that minimizes
// abs(X[i] - y).

type Strings struct {
	idx      map[int]string
	lst      []int
	unsorted int
}

func (st *Strings) Insert(s string, n int) {
	if st.idx == nil {
		st.idx = make(map[int]string)
		st.lst = make([]int, 0)
		st.unsorted = 0
	}
	st.idx[len(st.lst)] = s
	st.lst = append(st.lst, n)
	st.unsorted++
}

func (st *Strings) NearestTo(n int) (s string) {
	if st.unsorted > 0 {
		st.isort(st.unsorted)
	}
	st.unsorted = 0
	return st.search(0, len(st.lst), n)
}

func (st *Strings) linearNearestTo(n int) string {
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

func (st *Strings) search(i, j, n int) string {
	//p := (j - i) >> 1
	return ""
}
