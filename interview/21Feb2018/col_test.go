package col

import (
	"crypto/rand"
	"testing"
)

// A dumb metric for strings.
func metric(s string) float64 {
	var d []byte
	if len(s) < 4 {
		d = append([]byte(s), make([]byte, 4-len(s))...)
	} else {
		d = []byte(s)[:4]
	}
	return float64(uint(d[0])<<24 | uint(d[1])<<16 | uint(d[2])<<8 | uint(d[3]))
}

func TestInsertsNearest(t *testing.T) {
	targetCount := 10000
	testCount := 10000
	length := 10

	// Make a bunch of random, easy-to-read strings.
	testStrings := make([]string, targetCount+testCount)
	buf := make([]byte, length)
	for i, _ := range testStrings {
		if _, err := rand.Read(buf); err != nil {
			t.Fatal("rand.Read() fails:", err)
		}
		for j, x := range buf {
			buf[j] = byte((int(x) % 25) + 97) // Not uniform
		}
		testStrings[i] = string(buf)
	}

	// Insert the target strings into the collection.
	st := new(Strings)
	for i := 0; i < targetCount; i++ {
		st.Insert(testStrings[i], metric(testStrings[i]))
	}

	// For each test string, find the nearest in the collection according to the
	// metric.
	for i := targetCount; i < len(testStrings); i++ {
		_ = st.NearestTo(metric(testStrings[i]))
	}
}
