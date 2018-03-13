import Controller from '@ember/controller';

export default Controller.extend({
  session: Ember.inject.service(),
  ajax: Ember.inject.service(),
  actions: {
    display() {
      var that  = this;
      Ember.$.ajax({
        url: "/display_all",
        type: "POST",
        data: {
          "operator_id" : this.get('session').currentUser
        }
      }).then(function(resp){
          var table = document.getElementById("emptable");
          var rowCount = table.rows.length;
          for (var x=rowCount-1; x>=0; x--) {
             table.deleteRow(x);
          }
          $('#employee').css("display","none");
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
            var cell6 = row.insertCell(5);
            cell6.innerHTML = data[i][5];
          }
      }).catch(function(error){
        alert(JSON.stringify(error));
      });
    }
  }
});
