// ============================================================================
// HEADER COMPONENT
// Displays the plugin name and company branding
// ============================================================================

import React from 'react';
import config from '../../config';
import styles from './Header.module.css';

/**
 * Header Component
 * Displays the plugin name and company attribution
 */
function Header() {
  const { pluginName, companyName } = config;
  
  return (
    <header className={styles.container}>
      <h1 className={styles.title}>{pluginName}</h1>
      <p className={styles.subtitle}>by {companyName}</p>
    </header>
  );
}

export default Header;

