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
      	"fyne.io/fyne/v2/app"
      	"fyne.io/fyne/v2/widget"
        "bytes"
      //"net"
        "strings"
)



func window() {

}


func main() {


		logf := flag.String("log", "~/log_{datetime}.txt", "Output log filename, magic strings {datetime}")
		port := flag.String("port", "/dev/ttyUSB0", "Input port")
		port_baud := flag.Int("baud", 115200, "Input port baudrate")
		gui_en := flag.Bool("gui", false, "Eneable gui")
		//udp_port := flag.Int("upd", 3000, "UDP socket port")
		flag.Parse()


        logfile := stringFormatter.FormatComplex(*logf, map[string]interface{} {
            "datetime": strings.Replace(time.Now().UTC().Format(time.RFC3339), ":", "-", -1),
        })

        logfile, err :=filepath.Abs(logfile)

		fmt.Println(logfile)
		fmt.Println(*port)


		file, err := os.Create(logfile)
		if err != nil {
			log.Fatal(err)
		}

        file.Write([]byte(time.Now().UTC().Format(time.RFC3339) + "," +  "$LOGGER,golog,0.0,"+ "no-vcs" + "," + time.Now().UTC().Format(time.RFC3339) + "\n\r"))

		c := &serial.Config{Name: *port, Baud: *port_baud}
		s, err := serial.OpenPort(c)
		if err != nil {
			log.Fatal(err)
		}



        a := app.New()
        w := a.NewWindow("LABDOS data logger")
        if(*gui_en == true) {
            	w.SetContent(widget.NewLabel("LABDOS detector"))
        }

        go func() {
            for true {
                var content []byte
        		for true {
        			buf := make([]byte, 2048)
        			_, err := s.Read(buf)
        			if err != nil {
        				log.Fatal(err)
        			}
                    if bytes.Contains(buf, []byte("\r")){
                        //log.Print("KONEC EOL", n)
                        break
                    }
                    content = append(content, buf...)

                }
    			fmt.Print(time.Now().UTC().Format(time.RFC3339), ",", string(content), "\n\r")
    			file.Write( []byte(time.Now().UTC().Format(time.RFC3339) + "," + string(content) ))

            }
    	}()


        if(*gui_en == true) {
            	w.ShowAndRun()
        } else {
            for true {time.Sleep(1 * time.Second)}
        }



}
