import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vitejs.dev/config/
export default defineConfig({
  resolve: { alias: { '@': '/src' } },
  plugins: [vue()],
  server: {
    proxy: {
      '/api': {
        target: 'http://localhost:3689'
      },
      '/artwork': {
        target: 'http://localhost:3689'
      },
      '/stream.mp3': {
        target: 'http://localhost:3689'
      }
    }
  }
})
