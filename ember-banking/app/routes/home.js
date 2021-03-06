import Route from '@ember/routing/route';

export default Route.extend({
  session: Ember.inject.service(),
  beforeModel() {
    if(this.get('session').currentUser == null) {
      this.transitionTo('/login');
    }
    else {
      this.set('userID',this.get('session').currentUser);
    }
    alert("Welcome "+this.get('session').currentUser);
    this.set('userId',this.get('session').currentUser);
    //alert(this.get('userId'));
  }
});
