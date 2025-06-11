AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 1
   name "VoiceRange"
   tl -200 0
   children {
    3
   }
   value 0.8
  }
 }
 Ops {
  IOPItemOpInterpolateClass {
   id 3
   name "Interpolate 3"
   tl 3 115
   children {
    2
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
   "Y max" 100
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 2
   name "VoiveRange V"
   tl 200 0
   input 3
  }
 }
 compiled IOPCompiledClass {
  visited {
   5 7 6
  }
  ins {
   IOPCompiledIn {
    data {
     1 3
    }
   }
  }
  ops {
   IOPCompiledOp {
    data {
     1 2 2 0 0
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 3
  version 2
 }
}