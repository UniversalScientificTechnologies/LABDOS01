

pcb_length = 91;
pcb_width = 50.3;
pcb_thickness = 1.6+0.1;

wall = 3;

screw_positions = [[4.82, 40.56/2, 0], [4.82, -40.56/2, 0],
                  [4.82+7*10.16, 40.56/2, 0], [4.82+7*10.16, -40.56/2, 0]];


module basebox(){
translate([0, 0, wall-4]) minkowski(){
    cube([pcb_length, pcb_width, 10-wall]);
    sphere(d = wall*2, $fn=8);
}
}
                  

                  
module bot() translate([0, 0, -6.3]) {
    difference(){
    union(){
        basebox();
    }


    difference(){
        union(){
            // Vnitrni vyrez pro PCB 
            translate([0, 0, 0]) minkowski(){
                cube([pcb_length, pcb_width, 10]);
                cylinder(d = 0.4, h = 2, $fn=30);
            }
            
            // Vytvoreni rantlu
            difference(){
                translate([-10, -10, 6.3]) cube([pcb_length+20, pcb_width+20, 10]);
                translate([0, 0, 0]) minkowski(){
                    cube([pcb_length, pcb_width, 6.3]);
                    cylinder(d = 2.4, h = 2, $fn=8);
                }
            }
        }
        
        // Sloupky pro PCB
        translate([0, pcb_width/2, 0])
        for(p = screw_positions)
            translate(p){
                cylinder(d1 = 8, d2 = 8, h=6.3, $fn=20);
            }


        translate([pcb_length-8+0.5, 0, 0]) cube([8, pcb_width, 6.3-0.5]);
        
    }


    translate([0, pcb_width/2, -4])
        for(p = screw_positions)
            translate(p){
                translate([0, 0, 4.3]) cylinder(d = 3.3, h=15, $fn=20);
                cylinder(d = 6.5, h=4, $fn=6);
                cylinder(d1 = 6.5+1, d2=6.5, h=0.5, $fn=6);
            }
    
    
    
    translate([pcb_length, pcb_width/2+6, 4+1]) 
    minkowski() {cube([11, 35-2, 6], center=true);
        sphere(d=2, $fn=30);
    }
    
    
    translate([pcb_length+4, pcb_width/2+6, 4+1-6]) 
    minkowski() {cube([8, 35-2, 6], center=true);
        sphere(d=2, $fn=30);
    }
    
    
    
    
    
    // USB konektor
    
    translate([pcb_length, pcb_width/2, 4+1] + [-3, -17-2, 1]) 
    minkowski() {cube([11, 13-2, 6], center=true);
        sphere(d=2, $fn=30);
    }
    
    translate([pcb_length, pcb_width/2, 4+1] + [0, -18, 0]) 
    minkowski() {cube([11, 15, 6], center=true);
        sphere(d=2, $fn=30);
    }
    
    
    //translate([pcb_length, pcb_width/2, 4+1]) 
    //minkowski() {cube([11, 46-2, 6], center=true);
    //    sphere(d=2, $fn=30);
    //}
    
    
    }
    
}

module top(){

difference(){
    translate([0, 0, 0]) basebox();

    translate([-10, -10, -20]) cube([150, 150, 20.01]);
    
    
    difference(){
    difference(){
        union(){
            // Vnitrni vyrez pro PCB 
            translate([0, 0, 0]) minkowski(){
                cube([pcb_length, pcb_width, -0.1+2+5]);
                cylinder(d = 0.4, h = 0.1, $fn=30);
            }
            
            // zeslabeni nad diodou
            
            translate([14, pcb_width/2, 2]) minkowski(){
                cube([18-5, 30-5, -0.1+2+5], center=true);
                sphere(d = 5, $fn=60);
            }
            
           
                    
            // Vytvoreni rantlu
            intersection(){
                translate([-10, -10, 0]) cube([pcb_length+20, pcb_width+20, 2.5]);
                translate([0, 0, 0]) minkowski(){
                    cube([pcb_length, pcb_width, 6.3]);
                    cylinder(d = 2.4 + 0.3, h = 2, $fn=8);
                }
            }
      
       
    
        }
        translate([0, pcb_width/2, 0])
            for(p = screw_positions)
                translate(p+[0, 0, pcb_thickness+0.2]){
                    cylinder(d2 = 10, d1 = 8, h=7, $fn=20);
                }    
                
          
        // sloupek pro ledky
        
        translate([0, 0, pcb_thickness+0.2]) hull(){
            translate([49, 5, 0])
                        cylinder(d = 7, h=8, $fn=20);
            translate([49+20.05, 5, 0])
                        cylinder(d = 7, h=8, $fn=20);   
                }
                
        // PWR LED
        translate([0, 0+40.6, pcb_thickness+0.2]) hull(){
            translate([49+20.05, 5, 0])
                        cylinder(d = 7, h=8, $fn=20);   
                }
        }
  }
    
    
    translate([0, pcb_width/2, 0])
        for(p = screw_positions)
            translate(p){
                translate([0, 0, -5-0.3]) cylinder(d = 3.3, h=10, $fn=20);
                translate([0, 0, 9-4]) cylinder(d = 6, h=4, $fn=40);
                translate([0, 0, 9-0.5]) cylinder(d1 = 6, d2=6.3, h=0.3, $fn=40);
            }
            
    // Průzory pro LEDky
    for(x = [0:5:20])
        translate([49+x, 5, -11.25-0.2])
            hull(){
            translate([0, 0, 1]) cylinder(d1 = 4, d2=4, h=20, $fn=20);
            translate([0, 1, -8]) cylinder(d1 = 4, d2=4, h=20, $fn=20);
            translate([0, -1, -8]) cylinder(d1 = 4, d2=4, h=20, $fn=20);
            }
            
        translate([49+20, 5+40.6, -11.25-0.2])
            hull(){
            translate([0, 0, 1]) cylinder(d1 = 4, d2=4, h=20, $fn=20);
            translate([0, 1, -8]) cylinder(d1 = 4, d2=4, h=20, $fn=20);
            translate([0, -1, -8]) cylinder(d1 = 4, d2=4, h=20, $fn=20);
            }
            
            
    // Otvor pro SD kartu
    translate([pcb_length, pcb_width/2 +2.5+1, 1]) 
        minkowski() {
            cube([10, 32+5-2, 10], center=true);
            sphere(d=1.5, $fn=60);
        }
    
    // USB konektor 
    translate([pcb_length, pcb_width/2, -2] + [0, -18, 0]) 
    minkowski() {cube([11, 15, 6], center=true);
        sphere(d=2, $fn=30);
    }

    }
}



bot();
translate([0, -60, 0]) top();



module label(){
projection(cut= true)
    translate([0, 0, -9+0.3])
        top();
}

module label2(){
projection(cut= true)
    translate([0, 0, 10])
        bot();
}
