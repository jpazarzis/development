function bit_converter(){
       this.bit_flag = {};
       for(var i = 1; i <=20; ++i){
          this.bit_flag[i] = 1<< (i-1);
       }
       this.to_flag = function to_flag(arr){
          var flag = 0;    
          var length = arr.length; 
          for(var i =0; i < length; ++i){
              flag = flag | this.bit_flag[arr[i]];
          }
          return flag;
       } 

       this.to_array = function to_array(flag){
          var arr = [];
          for(var i = 1; i <=20; ++i){
             if ((this.bit_flag[i] & flag) == this.bit_flag[i]) {
                arr.push(i);
             }
          }
          return arr;
       }  
}

function unzip_combos(c){
    var arr = [];
    var bc = new bit_converter();
    for(var i = 0; i < c.length; ++i){
         var s = [];
         for(var j = 0; j < c[i].length; ++j){
            s.push(bc.to_array(c[i][j]));
         } 
         arr.push(s);
    }
    return arr;
}

function zip_combos(combinations){
   var c = [];
   var bc = new bit_converter();
   for(var i = 0; i < combinations.length; ++i){
        var x = [];
        for(var j = 0; j < combinations[i].length; ++j){
             x.push(bc.bit_flag[combinations[i][j]]);         
        }
        c.push(x);
   }
   return c;

}

function get_as_system(s){
   var str = '';
   for(var i = 0; i < s.length; ++i){     
     for(var j = 0; j < s[i].length; ++j){
          var n = ''+s[i][j];
          if(n.length == 1)
            n=' '+n;
          str += n + ' ';
     }
     str += '<br/>';
   }
   str += '<br/>';
   str += '<hr/>';     
   return str;
}

function get_diff(a1,a2){
    var length = a1.length;
    if(length != a2.length){
        return null;
    }
    var diff = [];
    for(var i = 0; i < length; ++i){
        if(a1[i] & a2[i] != a1[i]){
            diff.push(i);
        }   
    }
    return diff;
}

function compress(arr,position) {
    if(position >= arr.length-1){
        return;
    }
    while(true){
        var length_before = arr.length;
        for(var i = position+1 ; i < arr.length; ++i){
            
            var diff_indexes = get_diff(arr[position], arr[i]);
            if(diff_indexes.length == 1){
                var index = diff_indexes[0];
                arr[position][index] = arr[position][index] | arr[i][index];
                arr.splice(i,1);
                i = i -1;
            }   
        }
        if(length_before == arr.length){
            break;
        }            
    }
    compress(arr, position + 1);
}

function get_compressed(arr){
        arr = zip_combos(arr);
        while(true){
           var l1 = arr.length;
           compress(arr,0);
           if(l1 == arr.length) break;
        }
        arr = unzip_combos(arr);
        var s = '';
        for(var i = 0; i < arr.length; ++i){
            s += '<div>';

            s += get_as_system(arr[i]);
            s += '</div>';
        }
        return s;
}

var arr = [[1,1,1,1],[1,1,1,3],[1,1,1,4],[1,1,3,1],[1,1,3,3],[1,1,3,4],[1,3,1,1],[1,3,1,3],[1,3,1,4],[3,1,1,1],[3,1,1,3],[3,1,1,4],[3,1,3,1],[3,1,3,3],[3,1,3,4],[3,3,1,1],[3,3,1,3],[3,3,1,4]];
writeln(get_compressed(arr));


