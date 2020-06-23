//Библиотека управления ШД
#include <AccelStepper.h>  
                                                                           
//Порт данных тензорезистора и управляющего сигнала робота
#define Tenso A0                                                                                      
#define ControlSignal 5                                                                               
#define p1 2
#define p2 3
#define p3 4
#define p4 5

//Инициализация ШД для драйвера двигателя L298N
AccelStepper stepper(4, p1, p2, p3, p4);                                                              

void setup()  
{ 
  pinMode(Tenso, INPUT);
  pinMode(ControlSignal, INPUT);
  
  stepper.setMaxSpeed(500);                           
  stepper.setAcceleration(100);                                                    
}

void loop()
{  //Проверка управляющего сигнала
   if(digitalRead(ControlSignal)){                                                                          
   //Захват, пока управляющий сигнал HIGH                                                   
    while(digitalRead(ControlSignal)){                                                                   
   //Если объект не захвачен (показания тензодатчика не преодолевают порог давление), то выполняем захват
       if(analogRead(Tenso) < 50){                    
        stepper.setSpeed(450);
        while (analogRead(Tenso) < 50 && digitalRead(ControlSignal)) 
          stepper.runSpeed();                                                 
      }
   //Если объект захвачен, но с недостаточной силой, то замедляем вращение вала ШД
       else if(analogRead(Tenso) > 50 && analogRead(Tenso)<500){               
        stepper.setSpeed(10);                         
        while(analogRead(Tenso) != 0 && analogRead(Tenso)<500 && digitalRead(ControlSignal))
          stepper.runSpeed();   
       }
   //Если объект захвачен с достаточной силой, то останавливаем вращение вала ШД
       else{
        stepper.setSpeed(0);
        while(analogRead(Tenso)>500 && digitalRead(ControlSignal))
          stepper.runSpeed();
       }
    }
   }
   //Если управляющий сигнал равен LOW, то выполняем возврат в исходное положение
   else{                                              
    while(!(digitalRead(ControlSignal))){
      stepper.moveTo(0);
      stepper.runToPosition();
    } 
   }
}
