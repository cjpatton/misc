package col

import (
	"crypto/rand"
	"testing"
)

// A dumb metric for strings.
func score(s string) int {
	var d []byte
	if len(s) < 3 {
		d = append([]byte(s), make([]byte, 3-len(s))...)
	} else {
		d = []byte(s)[:3]
	}
	return int(d[0])<<24 | int(d[1])<<16 | int(d[2])<<8
}

func TestInsertsNearest(t *testing.T) {
	targetCount := 10
	testCount := 100
	length := 10

	st := new(Strings)
	buf := make([]byte, length)
	for i := 0; i < targetCount; i++ {
		if _, err := rand.Read(buf); err != nil {
			t.Fatal("rand.Read() fails:", err)
		}
		for j, x := range buf {
			buf[j] = byte((int(x) % 25) + 97) // Not uniform
		}
		s := string(buf)
		st.Insert(s, score(s))
	}

	for i := 0; i < testCount; i++ {
		if _, err := rand.Read(buf); err != nil {
			t.Fatal("rand.Read() fails:", err)
		}
		for j, x := range buf {
			buf[j] = byte((int(x) % 25) + 97) // Not uniform
		}
		s := string(buf)
		n := score(s)
		got := st.NearestTo(n)
		want := st.linearNearestTo(n)
		if got != want {
			t.Errorf("expected %s, got %s (n=%d)", want, got, n)
		}
	}
}
