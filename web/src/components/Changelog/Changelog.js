// ============================================================================
// CHANGELOG COMPONENT
// Displays version history with release notes and download links
// ============================================================================

import React, { useState, useEffect } from 'react';
import changelog from '../../config/changelog.json';
import config from '../../config';
import styles from './Changelog.module.css';

/**
 * Section type labels for display
 */
const SECTION_LABELS = {
  added: 'Added',
  changed: 'Changed',
  deprecated: 'Deprecated',
  removed: 'Removed',
  fixed: 'Fixed',
  security: 'Security'
};

/**
 * Section icons/emojis for visual distinction
 */
const SECTION_ICONS = {
  added: '✦',
  changed: '↻',
  deprecated: '⚠',
  removed: '✕',
  fixed: '✓',
  security: '🔒'
};

/**
 * Generate download URL for a specific version
 */
function getVersionDownloadUrl(version, platform) {
  const { downloadBaseUrl, repositoryName } = config;
  const platformDir = platform === 'mac' ? 'mac' : 'win';
  const ext = platform === 'mac' ? 'pkg' : 'exe';
  const platformLabel = platform === 'mac' ? 'Mac' : 'Win';
  return `${downloadBaseUrl}/${platformDir}/${repositoryName}${platformLabel}Installer_${version}.${ext}`;
}

/**
 * Format date string for display
 */
function formatDate(dateStr) {
  if (!dateStr) return '';
  const date = new Date(dateStr);
  return date.toLocaleDateString('en-US', { 
    year: 'numeric', 
    month: 'long', 
    day: 'numeric' 
  });
}

/**
 * Version Screenshot Component
 * Displays a thumbnail with lightbox support, or placeholder if missing
 */
function VersionScreenshot({ version, onOpenLightbox }) {
  const [imageError, setImageError] = useState(false);
  const screenshotPath = `${process.env.PUBLIC_URL}/screenshots/screenshot_${version}.png?v=${Date.now()}`;
  
  return (
    <div className={styles.versionScreenshot}>
      {imageError ? (
        <div className={styles.versionScreenshotPlaceholder}>
          Screenshot v{version}
        </div>
      ) : (
        <img 
          src={screenshotPath}
          alt={`Version ${version} screenshot - click to enlarge`}
          className={styles.versionScreenshotImg}
          onError={() => setImageError(true)}
          onClick={() => onOpenLightbox(screenshotPath, version)}
          title="Click to view full size"
        />
      )}
    </div>
  );
}

/**
 * Single version entry component
 */
function VersionEntry({ version, date, sections, isLatest, onOpenLightbox }) {
  const [expanded, setExpanded] = useState(isLatest);
  
  const sectionKeys = Object.keys(sections);
  const totalChanges = sectionKeys.reduce(
    (sum, key) => sum + sections[key].length, 
    0
  );

  const macUrl = getVersionDownloadUrl(version, 'mac');
  const winUrl = getVersionDownloadUrl(version, 'win');
  
  return (
    <div className={`${styles.version} ${isLatest ? styles.latest : ''}`}>
      <button 
        className={styles.versionHeader}
        onClick={() => setExpanded(!expanded)}
        aria-expanded={expanded}
      >
        <div className={styles.versionInfo}>
          <span className={styles.versionNumber}>v{version}</span>
          {isLatest && <span className={styles.latestBadge}>Latest</span>}
          <span className={styles.versionDate}>{formatDate(date)}</span>
        </div>
        <div className={styles.versionMeta}>
          <span className={styles.changeCount}>{totalChanges} change{totalChanges !== 1 ? 's' : ''}</span>
          <span className={styles.expandIcon}>{expanded ? '−' : '+'}</span>
        </div>
      </button>
      
      {expanded && (
        <div className={styles.versionContent}>
          {/* Screenshot for this version */}
          <VersionScreenshot version={version} onOpenLightbox={onOpenLightbox} />
          
          {/* Download links for this version */}
          <div className={styles.versionDownloads}>
            <span className={styles.downloadLabel}>Download v{version}:</span>
            <a href={macUrl} className={styles.downloadLink} download>
              macOS
            </a>
            <span className={styles.downloadSeparator}>|</span>
            <a href={winUrl} className={styles.downloadLink} download>
              Windows
            </a>
          </div>
          
          {sectionKeys.map(sectionKey => (
            <div key={sectionKey} className={styles.section}>
              <h4 className={styles.sectionTitle}>
                <span className={styles.sectionIcon}>{SECTION_ICONS[sectionKey] || '•'}</span>
                {SECTION_LABELS[sectionKey] || sectionKey}
              </h4>
              <ul className={styles.changeList}>
                {sections[sectionKey].map((item, index) => (
                  <li key={index} className={styles.changeItem}>{item}</li>
                ))}
              </ul>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}

/**
 * Changelog Component
 * Renders the full version history with lightbox support
 */
function Changelog() {
  const { versions } = changelog;
  const [lightbox, setLightbox] = useState({ open: false, src: '', version: '' });
  
  // Close lightbox on Escape key
  useEffect(() => {
    const handleEscape = (e) => {
      if (e.key === 'Escape') setLightbox({ open: false, src: '', version: '' });
    };
    if (lightbox.open) {
      document.addEventListener('keydown', handleEscape);
      document.body.style.overflow = 'hidden';
    }
    return () => {
      document.removeEventListener('keydown', handleEscape);
      document.body.style.overflow = '';
    };
  }, [lightbox.open]);
  
  const openLightbox = (src, version) => {
    setLightbox({ open: true, src, version });
  };
  
  const closeLightbox = () => {
    setLightbox({ open: false, src: '', version: '' });
  };
  
  if (!versions || versions.length === 0) {
    return null;
  }
  
  return (
    <section id="changelog" className={styles.container}>
      <h2 className={styles.title}>Version History</h2>
      
      <div className={styles.versions}>
        {versions.map((entry, index) => (
          <VersionEntry
            key={entry.version}
            version={entry.version}
            date={entry.date}
            sections={entry.sections}
            isLatest={index === 0}
            onOpenLightbox={openLightbox}
          />
        ))}
      </div>
      
      {/* Lightbox modal */}
      {lightbox.open && (
        <div className={styles.lightbox} onClick={closeLightbox}>
          <span className={styles.lightboxClose}>&times;</span>
          <img 
            src={lightbox.src}
            alt={`Version ${lightbox.version} screenshot`}
            className={styles.lightboxImage}
            onClick={(e) => e.stopPropagation()}
          />
        </div>
      )}
    </section>
  );
}

export default Changelog;

