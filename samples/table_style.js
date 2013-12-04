<!-- Javascript goes in the document HEAD -->
<script language="javascript" type="text/javascript">
function altRows(id){
    if(document.getElementsByTagName){  
        var tables = document.getElementsByTagName("table"); 
        for(j = 0; j < tables.length; j++){
                table = tables[j]
                var rows = table.getElementsByTagName("tr"); 

                for(i = 0; i < rows.length; i++){          
                    if(i % 2 == 0){
                        rows[i].className = "evenrowcolor";
                    }else{
                        rows[i].className = "oddrowcolor";
                    }      
                }
        }
    }
}

function doStuff() {
    
    altRows('alternatecolor');
}


</script>


