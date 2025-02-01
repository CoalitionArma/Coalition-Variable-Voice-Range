AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 18
   name "PlayerVoiceRange"
   tl -408.696 -177.391
  }
  IOPItemInputClass {
   id 3
   name "UnderPlayerControl"
   tl -404.523 -46.368
   children {
    10
   }
   value 1
  }
  IOPItemInputClass {
   id 12
   name "Amp Range 5"
   tl -412.36 -268.386
   global 1
  }
  IOPItemInputClass {
   id 13
   name "Amp Range 4"
   tl -413.074 -346.957
   global 1
  }
  IOPItemInputClass {
   id 14
   name "Amp Range 3"
   tl -412.36 -424.814
   global 1
  }
  IOPItemInputClass {
   id 15
   name "Amp Range 2"
   tl -412.36 -502.671
   global 1
  }
  IOPItemInputClass {
   id 16
   name "Amp Range 1"
   tl -410.931 -578.671
  }
 }
 Ops {
  IOPItemOpConditionClass {
   id 10
   name "!="
   comment "0 (non-spatialized) if entity is controlled by player"
   tl 13.333 -46.629
   ctl 0 -21
   inputs {
    ConnectionClass connection {
     id 3
     port 0
    }
   }
   "Condition Type" "!="
   Comparator 1
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 9
   name "Amp Out"
   tl 421.583 -300
  }
 }
 compiled IOPCompiledClass {
  version 2
 }
}