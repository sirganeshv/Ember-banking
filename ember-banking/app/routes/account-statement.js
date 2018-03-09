import Route from '@ember/routing/route';

export default Route.extend({
  ajax: Ember.inject.service(),
  actions: {
    display() {
      var that  = this;
      Ember.$.ajax({
        url: "/account_statement",
        type: "POST",
        data: {
          "acc_no" : $('#acc_no').val()
        }
      }).then(function(resp){
          var table = document.getElementById("emptable");
          var rowCount = table.rows.length;
          for (var x=rowCount-1; x>=0; x--) {
             table.deleteRow(x);
          }
          $('#employee').css("display","none");
          if(resp.trim() == 'Account number is not valid')
            alert("Invalid account number");
          else {
            var data = JSON.parse(resp);
            $('#employee').css("display","initial");
  					for(var i = 0; i < data.length;i++) {
  						var row = table.insertRow(i);
  						var cell1 = row.insertCell(0);
  						cell1.innerHTML = data[i][0];
  						var cell2 = row.insertCell(1);
              cell2.innerHTML = data[i][1];
  						var cell3 = row.insertCell(2);
  						cell3.innerHTML = data[i][2];
              var cell4 = row.insertCell(3);
  						cell4.innerHTML = data[i][3];
  						var cell5 = row.insertCell(4);
  						cell5.innerHTML = data[i][4];
  					}
          }
      }).catch(function(error){
        alert(JSON.stringify(error));
      });
    }
  }
});
