import Controller from '@ember/controller';

export default Controller.extend({
  ajax: Ember.inject.service(),
  actions: {
    display() {
      var that  = this;
      Ember.$.ajax({
        url: "/display",
        type: "POST",
        data: {
          "acc_no" : $('#acc_no').val()
        }
      }).then(function(resp){
          var table = document.getElementById("emptable");
          $('#employee').css("display","none");
          if(resp.trim() == 'Account number is not valid')
            alert("Invalid account number");
          else {
            table.deleteRow(0);
            var data = JSON.parse(resp);
            $('#employee').css("display","initial");
						var table = document.getElementById("emptable");
						var row = table.insertRow(0);
						var cell1 = row.insertCell(0);
						cell1.innerHTML = data[0];
						var cell2 = row.insertCell(1);
						cell2.innerHTML = data[1];
						var cell3 = row.insertCell(2);
						cell3.innerHTML = data[2];
						var cell4 = row.insertCell(3);
						cell4.innerHTML = data[3];
						var cell5 = row.insertCell(4);
						cell5.innerHTML = data[4];
						var cell6 = row.insertCell(5);
						cell6.innerHTML = data[5];
          }
      }).catch(function(error){
        alert(JSON.stringify(error));
      });
    }
  }
});
