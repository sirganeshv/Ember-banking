import Controller from '@ember/controller';

export default Controller.extend({
  session: Ember.inject.service(),
  ajax: Ember.inject.service(),
  actions: {
    forgot() {
      var that = this;
      alert(this.get('acc_no'));
      Ember.$.ajax({
        url: "/forgot_password",
        type: "POST",
        data: {
          "acc_no" : this.get('acc_no'),
          "operator_id" : this.get('session').currentUser
        }
      }).then(function(resp){
          var data  = resp;
          that.set('securityquestion',data);
          $('#security').css("display","initial");
					$('#answer').css("display","initial");
					$('#answerLabel').css("display","initial");
      }).catch(function(error){
          alert(JSON.stringify(error));
      });
    },
    security() {
      Ember.$.ajax({
        url: "/security",
        type: "POST",
        data: {
          "acc_no" : $('#acc_no').val(),
			    "answer" : $('#answer').val()
        }
      }).then(function(resp){
          var data  = resp;
          if(data.trim() == 'true') {
            $('#field').hide();
    				$('#answer').hide();
    				$('#answerLabel').hide();
    				$('#security').hide();
    				$('#securityquestion').hide();
    				$('#passphrase').css("display","initial");
    				$('#passphraseLabel').css("display","initial");
    				$('#change').css("display","initial");
          }
          else
            alert("Invalid data");
      }).catch(function(error){
          alert(JSON.stringify(error));
      });
    }
  }
});
