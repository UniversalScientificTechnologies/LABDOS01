package main

import (
      "github.com/tarm/serial"
      "path/filepath"
      "log"
	  "fmt"
	  "flag"
	  "os"
      "time"
      "github.com/wissance/stringFormatter"
      //"strings"
)

func main() {


		logf := flag.String("log", "~/log_{datetime}.txt", "Output log filename, magic strings {datetime}")
		port := flag.String("port", "/dev/ttyUSB0", "Input port")
		port_baud := flag.Int("baud", 115200, "Input port baudrate")
		flag.Parse()


        logfile := stringFormatter.FormatComplex(*logf, map[string]interface{} {
            "datetime": time.Now().UTC().Format(time.RFC3339),
        })

        // if strings.HasPrefix(logfile, "~/") {
        //     uhd, _ := os.UserHomeDir()
        //     logfile = filepath.Join(uhd, logfile[2:])
        // }

        logfile, err :=filepath.Abs(logfile)

		fmt.Println(logfile)
		fmt.Println(*port)


		file, err := os.Create(logfile)
		if err != nil {
			log.Fatal(err)
		}
		//deferfile.Close()

		c := &serial.Config{Name: *port, Baud: *port_baud}
		s, err := serial.OpenPort(c)
		if err != nil {
			log.Fatal(err)
		}

		for true {
			buf := make([]byte, 255)
			n, err := s.Read(buf)
			if err != nil {
				log.Fatal(err)
			}
			log.Print(time.Now().UTC().Format(time.RFC3339), string(buf[:n]))
			file.Write([]byte("  empty "))
			//file.Write(append([]byte(time.Now().UTC().Format(time.RFC3339))[:], buf[:n]))
		}
}
