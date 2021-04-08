import Vue from 'vue'
import app from './App.vue'
import $ from 'jquery'

new Vue({
  render: h => h(app),
}).$mount('#app')

require('./assets/js/jquery-3.6.0.min.js')

require('./assets/css/main.css')
require('./assets/css/jquery.terminal-2.22.0.min.css')

require('./assets/js/carve.js')
require('./assets/js/jquery.terminal-2.22.0.min.js')

console.log($)

var term = $('#console').terminal(function(cmd, term) {
  // Write to stdin
  //libcarve._buf_stdin += cmd + '\n';
}, {
  name: 'carve-console',
  greetings: '',
  prompt: '',
})