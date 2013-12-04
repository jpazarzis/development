<html>
<link rel="stylesheet" href="theme2.css" />
</head>

<script>

number_of_races = 6
selected_horses = [[], [], [], [], [], [] ]

unused_horses = [
                    [1,2,3,4,5,6,7,8,9,10,11,12,13,14], 
                    [1,2,3,4,5,6,7,8,9,10,11,12,13,14], 
                    [1,2,3,4,5,6,7,8,9,10,11,12,13,14], 
                    [1,2,3,4,5,6,7,8,9,10,11,12,13,14], 
                    [1,2,3,4,5,6,7,8,9,10,11,12,13,14], 
                    [1,2,3,4,5,6,7,8,9,10,11,12,13,14], 
                ]

limitations = []



function count_full_system(){
    fs = 1;
    for(i=0; i < selected_horses.length; ++i){
        var tokens = selected_horses[i];
        fs = fs * tokens.length;
    }
    return fs;
}

function create_add_horse_button(){
    var s = '<img src="Button-Add-icon.png" width="15" height="15" />';
    return s;
}

function move_horse(row_index,col_index,from_system_name,to_system_name){
    from_system= this[from_system_name];
    to_system= this[to_system_name];
    horse_number = from_system[row_index][col_index];
    from_system[row_index].splice(col_index,1);
    to_system[row_index].push(horse_number);
    update_screen();
}

function sortNumber(a,b) {
    return a - b;
}

function get_system_as_html(system_name, system2_name){
    system = this[system_name];
    system2 = this[system2_name];
    var s = '';
    for(i=0; i < system.length; ++i){
        s += '<tr>';
        s += '<td>' + (i+1) + '</td>';
        var tokens = system[i];
        tokens.sort(sortNumber);
        for(j=0; j < tokens.length; ++j){
            handler = 'onclick=\'move_horse(' + i + ','+j+',\"'+system_name +'\", \"' +system2_name  +'")\'';
            //handler = 'onclick="test()"'
            s += '<td><button ' +handler +'>' + tokens[j] + '</button></td>';
        }
        s += '</tr>';
    }
    return s;
}


function update_full_system(){
    var fst = document.getElementById('full_system');
    fst.innerHTML = get_system_as_html('selected_horses','unused_horses');
}

function update_available_horses(){
    var fst = document.getElementById('available_horses');
    fst.innerHTML = get_system_as_html('unused_horses','selected_horses');
}

function update_general_info(){
    var d = document.getElementById('general_info');
    d.innerHTML = count_full_system();
}

function update_screen(){
    update_full_system();
    update_available_horses();
    update_general_info();
}

function select_number_of_races(n){

    number_of_races = n

    selected_horses = []  
    for(var i = 0; i < n; ++i){
        selected_horses.push([]);
    }

    unused_horses = []
    for(var i = 0; i < n; ++i){
        unused_horses.push([]);
        for (var j = 1; j <=14; ++j){
            unused_horses[i].push(j); 
        }
    }

    update_screen();
}

function get_limitation_as_html(limitation){
    s = '<table>';
    for(i=0; i < limitation.length-2; ++i){
        s += '<tr>';
        k = i +1;
        s += '<td>' + k + '</td>';
        var tokens = limitation[i];
        tokens.sort(sortNumber);
        for(j=0; j < tokens.length; ++j){
            s += '<td><button>' + tokens[j] + '</button></td>';
        }
        s += '</tr>';
    }
    s += '</table>';
    return s;
}

function add_limitation(){
    
    limitation = selected_horses.slice(0)
    limitation.push(0)
    limitation.push(0)
    limitations.push(limitation)
    var div = document.getElementById('limitation_div');
    var element = document.createElement('span');
    element.innerHTML = get_limitation_as_html(limitation);
    div.appendChild(element);
}

</script>

</head>
<body onload="update_screen()">
<div id='general_info'>
</div>

<div>
 <p>
          <input type = "radio"
                 name = "radSize"
                 id = "sizeSmall"
                 value = "pick4"
                 onchange="select_number_of_races(4)"
                 checked = "checked" >pick4 </input>

          <input type = "radio"
                 name = "radSize"
                 id = "sizeSmall"
                 onchange="select_number_of_races(5)"
                 value = "pick5">pick5 </input> 

          <input type = "radio"
                 name = "radSize"
                 id = "sizeSmall"
                 onchange="select_number_of_races(6)"
                 value = "pick6">pick6</input> 
        </p>       
</div>


<div>
<span style='float:left;border:1px solid blue;margin-left:5px;margin-bottom:25px;background-color:#ddd;'>
Selected<br/>
<table id='full_system' style="line-height: 25px;">
</table>
</div>
</span>

<span style='float:left;border:1px solid blue;margin-left:5px;margin-bottom:25px;background-color:#ddd;'>
Available <br/>
<table id='available_horses'>
</table>
</span>
</div>
<br clear="all" /> 
<div>
<button onclick=add_limitation()>Add Limitation</button>
</div>
<div id='limitation_div'>
</div>


</body>
 </html>
