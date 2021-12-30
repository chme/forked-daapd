import { createApp } from 'vue'
import store from './store'
import VueProgressBar from '@aacassandra/vue3-progressbar'
import App from './App.vue'

createApp(App)
  .use(store)
  .use(VueProgressBar, {
    color: 'hsl(204, 86%, 53%)',
    failedColor: 'red',
    height: '1px'
  })
  .mount('#app')
