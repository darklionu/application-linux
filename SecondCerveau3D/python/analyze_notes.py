#!/usr/bin/env python3
"""
SecondCerveau3D - Analysis Engine
Analyzes agent data, threat intelligence, and network graph correlations
"""

import json
import sys
import argparse
from dataclasses import dataclass
from typing import Dict, List, Any, Optional
from datetime import datetime
import hashlib
import re

@dataclass
class ThreatIndicator:
    """Represents a threat indicator detected in the data"""
    indicator_type: str
    value: str
    severity: int  # 1-10
    source: str
    timestamp: str
    confidence: float

@dataclass
class AnalysisResult:
    """Result of analysis on a set of notes/data"""
    threat_score: int
    indicators: List[ThreatIndicator]
    correlations: List[Dict[str, Any]]
    recommendations: List[str]
    timestamp: str

class ThreatAnalyzer:
    """Main threat analysis engine"""
    
    def __init__(self):
        self.patterns = {
            'ip_address': r'\b(?:\d{1,3}\.){3}\d{1,3}\b',
            'port': r':(\d{1,5})',
            'hostname': r'(?:[a-z0-9](?:[a-z0-9-]{0,61}[a-z0-9])?\.)+[a-z]{2,}',
            'hash_md5': r'\b[a-f0-9]{32}\b',
            'hash_sha1': r'\b[a-f0-9]{40}\b',
            'hash_sha256': r'\b[a-f0-9]{64}\b',
        }
        
        self.threat_keywords = {
            'exploit': 5,
            'malware': 8,
            'backdoor': 9,
            'shellcode': 7,
            'payload': 6,
            'injection': 5,
            'overflow': 6,
            'vulnerability': 5,
            'attack': 4,
            'breach': 8,
            'unauthorized': 6,
        }
    
    def analyze_text(self, text: str, source: str = "unknown") -> AnalysisResult:
        """Analyze text content for threats and indicators"""
        
        indicators = []
        threat_score = 0
        
        # Extract indicators
        for indicator_type, pattern in self.patterns.items():
            matches = re.findall(pattern, text, re.IGNORECASE)
            for match in matches:
                threat_level = self._calculate_threat_level(indicator_type, match)
                indicator = ThreatIndicator(
                    indicator_type=indicator_type,
                    value=match,
                    severity=threat_level,
                    source=source,
                    timestamp=datetime.now().isoformat(),
                    confidence=0.8
                )
                indicators.append(indicator)
                threat_score += threat_level
        
        # Check for threat keywords
        text_lower = text.lower()
        for keyword, weight in self.threat_keywords.items():
            if keyword in text_lower:
                threat_score += weight
                indicators.append(ThreatIndicator(
                    indicator_type="keyword",
                    value=keyword,
                    severity=weight,
                    source=source,
                    timestamp=datetime.now().isoformat(),
                    confidence=0.9
                ))
        
        # Generate correlations
        correlations = self._find_correlations(indicators)
        
        # Generate recommendations
        recommendations = self._generate_recommendations(threat_score, indicators)
        
        # Normalize threat score to 0-100
        threat_score = min(100, threat_score)
        
        return AnalysisResult(
            threat_score=threat_score,
            indicators=indicators,
            correlations=correlations,
            recommendations=recommendations,
            timestamp=datetime.now().isoformat()
        )
    
    def _calculate_threat_level(self, indicator_type: str, value: str) -> int:
        """Calculate threat level based on indicator type"""
        
        threat_levels = {
            'ip_address': 3,
            'port': 2,
            'hostname': 3,
            'hash_md5': 4,
            'hash_sha1': 4,
            'hash_sha256': 4,
        }
        
        # Adjust based on specific values
        if indicator_type == 'port':
            port = int(value)
            if port < 1024:  # Privileged ports
                return 5
            if port in [22, 3389, 5900]:  # Remote access ports
                return 4
        
        return threat_levels.get(indicator_type, 2)
    
    def _find_correlations(self, indicators: List[ThreatIndicator]) -> List[Dict[str, Any]]:
        """Find correlations between indicators"""
        
        correlations = []
        
        # Group by type
        by_type = {}
        for indicator in indicators:
            if indicator.indicator_type not in by_type:
                by_type[indicator.indicator_type] = []
            by_type[indicator.indicator_type].append(indicator)
        
        # Find patterns
        for itype, items in by_type.items():
            if len(items) > 2:
                correlations.append({
                    'type': 'frequency',
                    'indicator_type': itype,
                    'count': len(items),
                    'severity': 'medium' if len(items) < 5 else 'high'
                })
        
        return correlations
    
    def _generate_recommendations(self, threat_score: int, indicators: List[ThreatIndicator]) -> List[str]:
        """Generate security recommendations"""
        
        recommendations = []
        
        if threat_score >= 80:
            recommendations.append("🔴 CRITICAL: Activate blue team response protocols immediately")
            recommendations.append("🔴 Initiate incident response procedures")
            recommendations.append("🔴 Isolate affected systems")
        elif threat_score >= 50:
            recommendations.append("🟠 HIGH: Increase monitoring on identified indicators")
            recommendations.append("🟠 Prepare incident response team")
        elif threat_score >= 20:
            recommendations.append("🟡 MEDIUM: Review identified indicators")
            recommendations.append("🟡 Update threat intelligence feeds")
        else:
            recommendations.append("🟢 LOW: Continue normal monitoring")
        
        # Type-specific recommendations
        for indicator in indicators:
            if indicator.indicator_type == 'hash_sha256' and indicator.severity >= 4:
                recommendations.append(f"Block hash {indicator.value[:16]}... at firewall")
            elif indicator.indicator_type == 'ip_address' and indicator.severity >= 4:
                recommendations.append(f"Monitor connections to {indicator.value}")
        
        return list(set(recommendations))  # Remove duplicates
    
    def analyze_agent_data(self, agent_data: Dict[str, Any]) -> AnalysisResult:
        """Analyze structured agent data from orchestrator"""
        
        # Combine all text fields from agent data
        text_parts = []
        
        if 'scan_results' in agent_data:
            text_parts.append(json.dumps(agent_data['scan_results']))
        if 'findings' in agent_data:
            text_parts.append(agent_data['findings'])
        if 'alerts' in agent_data:
            for alert in agent_data.get('alerts', []):
                text_parts.append(alert.get('message', ''))
        
        combined_text = ' '.join(text_parts)
        return self.analyze_text(combined_text, source=agent_data.get('agent_type', 'unknown'))

class ReportGenerator:
    """Generate analysis reports"""
    
    @staticmethod
    def generate_json_report(result: AnalysisResult) -> str:
        """Generate JSON report"""
        
        report = {
            'timestamp': result.timestamp,
            'threat_score': result.threat_score,
            'indicator_count': len(result.indicators),
            'indicators': [
                {
                    'type': ind.indicator_type,
                    'value': ind.value,
                    'severity': ind.severity,
                    'confidence': ind.confidence,
                    'source': ind.source
                } for ind in result.indicators
            ],
            'correlations': result.correlations,
            'recommendations': result.recommendations
        }
        
        return json.dumps(report, indent=2)
    
    @staticmethod
    def generate_text_report(result: AnalysisResult) -> str:
        """Generate human-readable text report"""
        
        lines = [
            "=" * 60,
            "SECONDCERVEAU3D - THREAT ANALYSIS REPORT",
            "=" * 60,
            f"Timestamp: {result.timestamp}",
            f"Overall Threat Score: {result.threat_score}/100",
            "",
            "IDENTIFIED INDICATORS:",
            "-" * 40,
        ]
        
        for ind in result.indicators:
            lines.append(f"  [{ind.indicator_type}] {ind.value} (Severity: {ind.severity}/10)")
        
        lines.extend([
            "",
            "CORRELATIONS:",
            "-" * 40,
        ])
        
        for corr in result.correlations:
            lines.append(f"  {corr['type']}: {corr}")
        
        lines.extend([
            "",
            "RECOMMENDATIONS:",
            "-" * 40,
        ])
        
        for rec in result.recommendations:
            lines.append(f"  • {rec}")
        
        lines.append("=" * 60)
        
        return '\n'.join(lines)

def main():
    parser = argparse.ArgumentParser(description='SecondCerveau3D Threat Analysis Engine')
    parser.add_argument('--file', help='File to analyze')
    parser.add_argument('--text', help='Text to analyze')
    parser.add_argument('--format', choices=['json', 'text'], default='text', help='Output format')
    parser.add_argument('--agent-data', help='Agent data JSON file')
    
    args = parser.parse_args()
    
    analyzer = ThreatAnalyzer()
    
    if args.agent_data:
        with open(args.agent_data, 'r') as f:
            agent_data = json.load(f)
        result = analyzer.analyze_agent_data(agent_data)
    elif args.file:
        with open(args.file, 'r') as f:
            text = f.read()
        result = analyzer.analyze_text(text, source=args.file)
    elif args.text:
        result = analyzer.analyze_text(args.text)
    else:
        parser.print_help()
        return 1
    
    # Generate report
    report_gen = ReportGenerator()
    if args.format == 'json':
        print(report_gen.generate_json_report(result))
    else:
        print(report_gen.generate_text_report(result))
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
