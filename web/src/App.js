// ============================================================================
// MAIN APPLICATION COMPONENT
// ============================================================================

import React from 'react';
import HelloWorld from './components/HelloWorld/HelloWorld';
import './App.css';

/**
 * App Component
 * Root component that orchestrates the application layout
 */
function App() {
  return (
    <div className="app">
      <main className="app__main">
        <HelloWorld />
      </main>
    </div>
  );
}

export default App;

